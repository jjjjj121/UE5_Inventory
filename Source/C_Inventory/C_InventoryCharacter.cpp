// Copyright Epic Games, Inc. All Rights Reserved.

#include "C_InventoryCharacter.h"
#include "C_Inventory/Public/Actors/Item.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DrawDebugHelpers.h"
#include "InputMappingContext.h"
#include "Net/UnrealNetwork.h"

//////////////////////////////////////////////////////////////////////////
// AC_InventoryCharacter

AC_InventoryCharacter::AC_InventoryCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	/*맵핑 설정 추가?*/
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>DEFAULT_CONTEXT
	(TEXT("/Game/ThirdPerson/Input/IMC_Default.IMC_Default"));
	if (DEFAULT_CONTEXT.Succeeded())
		DefaultMappingContext = DEFAULT_CONTEXT.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Interact
	(TEXT("/Game/ThirdPerson/Input/Actions/IA_Interact.IA_Interact"));
	if (IA_Interact.Succeeded())
		InteractAction = IA_Interact.Object;

}

void AC_InventoryCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AC_InventoryCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	/*아래 매크로 사용하기 위해서 Net/UnrealNetwork.h 포함 필요*/
	/*아래 매크로를 통해 replication의 세밀한 제어를 위한 부차적인 조건을 추가시킬 수 있다.*/
	DOREPLIFETIME_CONDITION(AC_InventoryCharacter, InventoryItems, COND_OwnerOnly);

}

void AC_InventoryCharacter::AddInventoryItem(FItemData ItemData)
{
	if (HasAuthority()) {

		InventoryItems.Add(ItemData);
	}
	
}

void AC_InventoryCharacter::AddHealth(float Value)
{
	Health += Value;
	UpdateStats(Hunger, Health);
	UE_LOG(LogTemp, Warning, TEXT("ADDED HEALTH : %f"), Health);

}

void AC_InventoryCharacter::RemoveHunger(float Value)
{
	Hunger -= Value;
	UpdateStats(Hunger, Health);
	UE_LOG(LogTemp, Warning, TEXT("REMOVED HEALTH : %f"), Hunger);

}

//////////////////////////////////////////////////////////////////////////
// Input

void AC_InventoryCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		/*Interact*/
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AC_InventoryCharacter::Interact);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AC_InventoryCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AC_InventoryCharacter::Look);

	}

}

void AC_InventoryCharacter::Interact()
{

	FVector Start = FollowCamera->GetComponentLocation();
	FVector End = Start + FollowCamera->GetForwardVector() * 500.0f;
	
	/*서버일 경우 기존 interact 함수 사용*/
	if (HasAuthority()) {
		Interact(Start, End);
	}
	/*클라일 경우 Server_Interact를 통해 서버에서 interact 실행*/
	/*왜 ? -> 서버에서 실행되어야 replicate 되어서 서버와 클라 모두 동기화 되기때문*/
	else {
		Server_Interact(Start, End);
	}

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f);

}

void AC_InventoryCharacter::Interact(FVector Start, FVector End)
{

	FHitResult HitResult;

	/*콜리전 세팅*/
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	/*라인트레이스 세팅*/
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		/*HitResult 가 True 면 엑터이름 출력*/
		if (IInteractInterface* Interface = Cast<IInteractInterface>(HitResult.GetActor())) {

			Interface->Interact(this);
			//UE_LOG(LogTemp, Warning, TEXT("Hit Actor : %s"), *Actor->GetName());
		}
	}
}

bool AC_InventoryCharacter::Server_Interact_Validate(FVector Start, FVector End)
{
	return true;
}

void AC_InventoryCharacter::Server_Interact_Implementation(FVector Start, FVector End){

	Interact(Start, End);

}



void AC_InventoryCharacter::UpdateStats_Implementation(float NewHunger, float NewHealth)
{
}

void AC_InventoryCharacter::OnRep_InventoryItems()
{
	/*Inventory에 Item이 존재할 경우*/
	if (InventoryItems.Num()) {
		
		/*뒤에 추가되어야 하기 떄문에 Num() -1로 마지막 인덱스 가리킴*/
		AddItemToInventory(InventoryItems[InventoryItems.Num() - 1]);
	}



}

void AC_InventoryCharacter::UseItem(TSubclassOf<AItem> ItemSubclass)
{
	if (ItemSubclass) {
		if (AItem* CDOItem = ItemSubclass.GetDefaultObject()) {
			CDOItem->Use(this);
		}
	}

}


void AC_InventoryCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AC_InventoryCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}







