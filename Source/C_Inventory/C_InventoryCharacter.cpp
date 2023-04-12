// Copyright Epic Games, Inc. All Rights Reserved.

#include "C_InventoryCharacter.h"
#include "C_Inventory/Public/Actors/Item.h"
#include "C_Inventory/Public/Actors/Gold.h"
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
#include "C_Inventory/Public/Actors/ShopKeeper.h"
#include "C_Inventory/Public/Widget/switchingwidget.h"
#include "C_Inventory/Public/Widget/Inventory.h"
#include "C_Inventory/Public/Widget/Shop.h"
#include "C_Inventory/Public/Widget/TradeWidget.h"


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

	/*Create Widget*/
	if (IsLocallyControlled()) {
		HUDWidget = Cast<Uswitchingwidget>(CreateWidget(GetWorld(), WidgetClass));
		if (HUDWidget) {
			HUDWidget->AddToViewport();
		}
	}

}

void AC_InventoryCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/*아래 매크로 사용하기 위해서 Net/UnrealNetwork.h 포함 필요*/
	/*아래 매크로를 통해 replication의 세밀한 제어를 위한 부차적인 조건을 추가시킬 수 있다.*/
	DOREPLIFETIME_CONDITION(AC_InventoryCharacter, InventoryItems, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AC_InventoryCharacter, TradeItems, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AC_InventoryCharacter, MyGold, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AC_InventoryCharacter, TradeGold, COND_OwnerOnly);
	DOREPLIFETIME(AC_InventoryCharacter, Health);
	DOREPLIFETIME(AC_InventoryCharacter, Hunger);
	DOREPLIFETIME(AC_InventoryCharacter, WantTrade);
	DOREPLIFETIME(AC_InventoryCharacter, bRunningTrade);
}


int32 AC_InventoryCharacter::GetGold()
{
	return MyGold;
}

int32 AC_InventoryCharacter::GetTradeGold()
{
	return TradeGold;
}


void AC_InventoryCharacter::AddInventoryItem(FItemData ItemData, bool IsInventoryItem)
{
	if (HasAuthority()) {
		UE_LOG(LogTemp, Warning, TEXT("3"));
		/*Inventory*/
		if (IsInventoryItem) {
			bool bIsNewItem = true;
			/*아이템이 골드일 경우*/
			if (ItemData.ItemClass == AGold::StaticClass()) {
				MyGold += ItemData.StackCount;
				bIsNewItem = false;
			}
			/*골드 제외 아이템일 경우*/
			else {
				for (FItemData& Item : InventoryItems) {
					if (Item.ItemClass == ItemData.ItemClass) {
						if (ItemData.StackCount > 1) {
							Item.StackCount += ItemData.StackCount;
						}
						else {
							++Item.StackCount;
						}

						bIsNewItem = false;
						break;
					}
				}
			}
			/*인벤토리에 없는 아이템일 경우 새로 추가*/
			if (bIsNewItem) {
				InventoryItems.Add(ItemData);
			}
			/*로컬일 경우*/
			if (IsLocallyControlled()) {
				OnRep_InventoryItems();
			}
		}
		/*TradeWidget*/
		else {
			bool bIsNewItem = true;
			/*아이템이 골드일 경우*/
			if (ItemData.ItemClass == AGold::StaticClass()) {
				/*TEST*/
				UpdateGold(ItemData.StackCount, false);

				TradeGold += ItemData.StackCount;
				UE_LOG(LogTemp, Warning, TEXT("4"));
				bIsNewItem = false;
			}
			/*골드 제외 아이템일 경우*/
			else {
				for (FItemData& Item : TradeItems) {
					if (Item.ItemClass == ItemData.ItemClass) {
						if (ItemData.StackCount > 1) {
							Item.StackCount += ItemData.StackCount;
						}
						else {
							++Item.StackCount;
						}
						bIsNewItem = false;
						break;
					}
				}
			}
			/*인벤토리에 없는 아이템일 경우 새로 추가*/
			if (bIsNewItem) {
				TradeItems.Add(ItemData);
			}
			/*로컬일 경우*/
			if (IsLocallyControlled()) {
				UE_LOG(LogTemp, Warning, TEXT("5"));
				OnRep_TradeItems();
			}
		}

	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("1"));
		Server_AddInventoryItem(ItemData, IsInventoryItem);
	}

}

bool AC_InventoryCharacter::Server_AddInventoryItem_Validate(FItemData ItemData, bool IsInventoryItem)
{
	return true;
}

void AC_InventoryCharacter::Server_AddInventoryItem_Implementation(FItemData ItemData, bool IsInventoryItem)
{
	UE_LOG(LogTemp, Warning, TEXT("2"));
	AddInventoryItem(ItemData, IsInventoryItem);

}
void AC_InventoryCharacter::AddHealth(float Value)
{
	Health += Value;
	if (IsLocallyControlled()) {
		UpdateStats(Hunger, Health);
	}

}

void AC_InventoryCharacter::RemoveHunger(float Value)
{
	Hunger -= Value;
	if (IsLocallyControlled()) {
		UpdateStats(Hunger, Health);
	}

}

void AC_InventoryCharacter::UpdateGold(int32 GoldValue, bool IsMyGold)
{
	Server_UpdateGold(GoldValue, IsMyGold);

	if (IsLocallyControlled()) {
		OnRep_InventoryItems();
		OnRep_TradeItems();
	}


}

void AC_InventoryCharacter::Server_UpdateGold_Implementation(int32 GoldValue, bool IsMyGold)
{
	IsMyGold ? TradeGold += GoldValue : MyGold += GoldValue;
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
		Interact(Start, End);
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
		/*맞은 엑터가 상점*/
		AShopKeeper* ShopKeeper = Cast<AShopKeeper>(HitResult.GetActor());
		if (ShopKeeper) {
			if (IsLocallyControlled()) {
				ShopKeeper->Interact(this);
			}
			return;
		}
		/*Trade*/
		AC_InventoryCharacter* UserCharacter = Cast<AC_InventoryCharacter>(HitResult.GetActor());
		if (UserCharacter) {
			TryTrade(UserCharacter);
		}
		/*Item*/
		if (IInteractInterface* Interface = Cast<IInteractInterface>(HitResult.GetActor())) {
			Interface->Interact(this);
		}

	}
}



bool AC_InventoryCharacter::Server_Interact_Validate(FVector Start, FVector End)
{
	return true;
}

void AC_InventoryCharacter::Server_Interact_Implementation(FVector Start, FVector End) {

	Interact(Start, End);

}

void AC_InventoryCharacter::UpdateStats_Implementation(float NewHunger, float NewHealth)
{
	if (HUDWidget) {
		HUDWidget->UpdateStats(NewHunger, NewHealth);
	}
}



void AC_InventoryCharacter::OnRep_Stats()
{
	if (IsLocallyControlled()) {
		UpdateStats(Hunger, Health);
	}

}

void AC_InventoryCharacter::OnRep_InventoryItems()
{
	/*Inventory에 Item이 존재할 경우*/
	if (InventoryItems.Num()) {
		/*최근 아이템이 추가되어야 하기 떄문에 Num() -1로 마지막 인덱스 가리킴*/
		AddItemAndUpdateInventory(InventoryItems[InventoryItems.Num() - 1], InventoryItems);
	}
	else {
		AddItemAndUpdateInventory(FItemData(), InventoryItems);

	}
}

void AC_InventoryCharacter::AddItemAndUpdateInventory_Implementation(FItemData ItemData, const TArray<FItemData>& NewInventoryItems)
{
	if (HUDWidget) {
		/*Gold Update*/
		HUDWidget->W_Inventory->UpdateGold(MyGold);

		/*Item일 경우*/
		if (HUDWidget->W_Inventory->IsNewItem(NewInventoryItems)) {
			HUDWidget->W_Inventory->AddItem(ItemData);
		}
		else {
			HUDWidget->W_Inventory->Update(NewInventoryItems);
		}

	}
}

void AC_InventoryCharacter::OnRep_TradeItems()
{
	/*TradeWidget에 Item이 존재할 경우*/
	if (TradeItems.Num()) {
		/*최근 아이템이 추가되어야 하기 떄문에 Num() -1로 마지막 인덱스 가리킴*/
		AddItemAndUpdateTradeWidget(TradeItems[TradeItems.Num() - 1], TradeItems);
	}
	else {
		AddItemAndUpdateTradeWidget(FItemData(), TradeItems);

	}
}

void AC_InventoryCharacter::User_TradeItem(const TArray<FItemData>& NewTradeWidgetItems, bool IsMyTradeSlot)
{
	if (HasAuthority()) {
		Multi_User_TradeItems(NewTradeWidgetItems, IsMyTradeSlot);
	}
	else {
		Server_User_TradeItems(NewTradeWidgetItems, IsMyTradeSlot);
	}

}



void AC_InventoryCharacter::Multi_User_TradeItems_Implementation(const TArray<FItemData>& NewTradeWidgetItems, bool IsMyTradeSlot)
{
	if (IsLocallyControlled()) {
		/*TradeWidget에 Item이 존재할 경우*/
		if (NewTradeWidgetItems.Num()) {
			/*최근 아이템이 추가되어야 하기 떄문에 Num() -1로 마지막 인덱스 가리킴*/
			AddItemAndUpdateTradeWidget(NewTradeWidgetItems[NewTradeWidgetItems.Num() - 1], NewTradeWidgetItems, IsMyTradeSlot);
		}
		else {
			AddItemAndUpdateTradeWidget(FItemData(), NewTradeWidgetItems, IsMyTradeSlot);

		}
	}
}

void AC_InventoryCharacter::Server_User_TradeItems_Implementation(const TArray<FItemData>& NewTradeWidgetItems, bool IsMyTradeSlot)
{
	Multi_User_TradeItems(NewTradeWidgetItems, IsMyTradeSlot);
}

void AC_InventoryCharacter::AddItemAndUpdateTradeWidget(FItemData ItemData, const TArray<FItemData>& NewTradeWidgetItems, bool IsMyTradeSlot)
{
	if (HUDWidget) {
		/*Gold Update*/
		HUDWidget->W_TradeWidget->UpdateGold(TradeGold, true);

		/*Item일 경우*/
		if (HUDWidget->W_TradeWidget->IsNewItem(NewTradeWidgetItems, IsMyTradeSlot)) {

			HUDWidget->W_TradeWidget->AddItem(ItemData, IsMyTradeSlot);
		}
		else {
			HUDWidget->W_TradeWidget->Update(NewTradeWidgetItems, IsMyTradeSlot);
		}
	}
}

void AC_InventoryCharacter::UseItem(TSubclassOf<AItem> ItemSubclass, AShopKeeper* ShopKeeper, bool IsShopItem, bool IsTradeWidgetItem)
{
	if (ItemSubclass) {
		if (HasAuthority()) {
			if (!ShopKeeper) {
				/*While Trade*/
				if (bRunningTrade) {
					UE_LOG(LogTemp, Warning, TEXT("TRADE RUNNING : %d"), IsTradeWidgetItem);
					if (AItem* Item = ItemSubclass.GetDefaultObject()) {
						Item->Use(this, IsShopItem, IsTradeWidgetItem);
					}
					uint8 Index = 0;
					FItemData TempData;
					
					TArray<FItemData>& MyItems = IsTradeWidgetItem ? TradeItems : InventoryItems;


					for (FItemData& Item : MyItems) {
						if (Item.ItemClass == ItemSubclass) {
							--Item.StackCount;
							TempData = Item;
							if (Item.StackCount <= 0) {
								MyItems.RemoveAt(Index);
							}
							break;
						}
						++Index;
					}
					if (TempData.ItemClass) {
						TempData.StackCount = 1;
						IsTradeWidgetItem ? AddInventoryItem(TempData) : AddInventoryItem(TempData, false);
						
					}
					if (IsLocallyControlled()) {
						OnRep_InventoryItems();
						OnRep_TradeItems();
						
					}
					if (TradeCharacter) {
						TradeCharacter->User_TradeItem(TradeItems, false);
					}

				}
				else {
					if (AItem* Item = ItemSubclass.GetDefaultObject()) {
						Item->Use(this, IsShopItem, IsTradeWidgetItem);
					}
					uint8 Index = 0;
					for (FItemData& Item : InventoryItems) {
						if (Item.ItemClass == ItemSubclass) {
							--Item.StackCount;
							/*아이템을 모두 소모했을 경우*/
							if (Item.StackCount <= 0) {
								InventoryItems.RemoveAt(Index);
							}
							break;
						}
						++Index;
					}
					if (IsLocallyControlled()) {
						OnRep_InventoryItems();
						OnRep_TradeItems();
					}
				}


			}
			/*Shop*/
			else {
				if (ShopKeeper->CanBuyItem(MyGold, ItemSubclass)) {
					if (AItem* Item = ItemSubclass.GetDefaultObject()) {
						Item->Use(this, IsShopItem, IsTradeWidgetItem);
						ShopKeeper->BuyItem(this, ItemSubclass);
					}
				}
			}

		}
		else {

			Server_UseItem(ItemSubclass, ShopKeeper, IsShopItem, IsTradeWidgetItem);
		}
	}

}


bool AC_InventoryCharacter::Server_UseItem_Validate(TSubclassOf<AItem> ItemSubclass, AShopKeeper* ShopKeeper, bool IsShopItem, bool IsTradeWidgetItem)
{
	/*아래 주석의 경우는 버그를 이용해 아이템을 사용하려는 경우 서버에서 킥을 하기 위함*/
	///*인벤토리 배열에 사용하려는 아이템이 존재할 경우*/
	//for (FItemData& Item : InventoryItems) {
	//	if (Item.ItemClass == ItemSubclass) {

	//		return true;
	//	}
	//}
	///*인벤토리에 존재하지 않은 아이템을 사용하려고 한 경우*/
	//return false;

	return true;
}

void AC_InventoryCharacter::Server_UseItem_Implementation(TSubclassOf<AItem> ItemSubclass, AShopKeeper* ShopKeeper, bool IsShopItem, bool IsTradeWidgetItem)
{
	
	if (IsShopItem) {
		UseItem(ItemSubclass, ShopKeeper, IsShopItem, IsTradeWidgetItem);
	}
	else {
		if (bRunningTrade) {
			UseItem(ItemSubclass, ShopKeeper, IsShopItem, IsTradeWidgetItem);
		}
		else {
			/*인벤토리 배열에 사용하려는 아이템이 존재할 경우*/
			for (FItemData& Item : InventoryItems) {
				if (Item.ItemClass == ItemSubclass) {
					if (Item.StackCount) {
						UseItem(ItemSubclass, ShopKeeper, IsShopItem, IsTradeWidgetItem);
					}

					return;
				}
			}
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


void AC_InventoryCharacter::SwitchingUI()
{
	if (HUDWidget) {
		if ((HUDWidget->GetActiveWidgetIndex() == 0)) {
			HUDWidget->SwitchingUI(1);
		}
		else {
			HUDWidget->SwitchingUI(0);
		}
	}
}


void AC_InventoryCharacter::OpenShop_Implementation(const TArray<FItemData>& Items, AShopKeeper* ShopKeeper)
{
	if (HUDWidget) {
		if (!HUDWidget->W_Shop->IsVisible()) {
			HUDWidget->W_Shop->AddShopItem(Items, ShopKeeper);
			HUDWidget->OpenShopUI();
		}
	}
}

void AC_InventoryCharacter::UpdateShop_Implementation(const TArray<FItemData>& Items)
{
	if (HUDWidget) {
		HUDWidget->W_Shop->UpdateShop(Items);
	}
}



void AC_InventoryCharacter::OnTrade(AC_InventoryCharacter* TradeUser)
{

	if (HasAuthority()) {
		TradeCharacter = TradeUser;
		SetRunningTrade(true);
		if (IsLocallyControlled()) {
			HUDWidget->OnTrade(TradeUser);
		}
		else {
			Client_OnTrade(TradeUser);
		}
	}
	else {
		if (IsLocallyControlled()) {
			HUDWidget->OnTrade(TradeUser);
			SetRunningTrade(true);
		}
	}

}

void AC_InventoryCharacter::EndTrade()
{
	SetWantTrade(false);
	SetRunningTrade(false);
	if (IsLocallyControlled()) {
		HUDWidget->EndTrade();
	}
	else {
		Client_EndTrade();
	}
}

bool AC_InventoryCharacter::Client_EndTrade_Validate()
{
	return true;
}

void AC_InventoryCharacter::Client_EndTrade_Implementation()
{
	EndTrade();
}

void AC_InventoryCharacter::EndTrade(AC_InventoryCharacter* TradeUser)
{
	Server_EndTrade(TradeUser);
}


bool AC_InventoryCharacter::Server_EndTrade_Validate(AC_InventoryCharacter* TradeUser)
{
	return true;
}
void AC_InventoryCharacter::Server_EndTrade_Implementation(AC_InventoryCharacter* TradeUser)
{
	TradeUser->EndTrade();
}

bool AC_InventoryCharacter::Client_OnTrade_Validate(AC_InventoryCharacter* TradeUser)
{
	return true;
}

void AC_InventoryCharacter::Client_OnTrade_Implementation(AC_InventoryCharacter* TradeUser)
{
	OnTrade(TradeUser);
}

void AC_InventoryCharacter::ClientTryTrade(AC_InventoryCharacter* TradeUser)
{
	Server_ClientTryTrade(TradeUser);
}


bool AC_InventoryCharacter::Server_ClientTryTrade_Validate(AC_InventoryCharacter* TradeUser)
{
	return true;
}

void AC_InventoryCharacter::Server_ClientTryTrade_Implementation(AC_InventoryCharacter* TradeUser)
{
	TradeUser->TryTrade(this);
}


void AC_InventoryCharacter::SetRunningTrade(bool NewValue)
{
	if (HasAuthority()) {
		bRunningTrade = NewValue;
	}
	else {
		Server_SetRunningTrade(NewValue);
	}
}

void AC_InventoryCharacter::SetUserTradeGold(int32 GoldValue)
{
	if (bRunningTrade) {
		if (HasAuthority()) {
			Multicast_SetUserTradeGold(GoldValue);
		}
	}
}

void AC_InventoryCharacter::Multicast_SetUserTradeGold_Implementation(int32 GoldValue)
{
	if (IsLocallyControlled()) {
		HUDWidget->UpdateTradeGold(GoldValue, false);
	}
}

void AC_InventoryCharacter::ClientSetUserTradeGold(int32 GoldValue)
{
	if (HUDWidget) {
		Server_ClientSetUserTradeGold(HUDWidget->TradeCharacter, GoldValue);
	}
	
}

void AC_InventoryCharacter::Server_ClientSetUserTradeGold_Implementation(AC_InventoryCharacter* NewTradeCharacter, int32 GoldValue)
{
	if (NewTradeCharacter) {
		NewTradeCharacter->SetUserTradeGold(GoldValue);
	}
	
}


void AC_InventoryCharacter::Server_SetRunningTrade_Implementation(bool NewValue)
{
	bRunningTrade = NewValue;
}

void AC_InventoryCharacter::SetWantTrade(bool NewValue)
{
	if (HasAuthority()) {
		WantTrade = NewValue;
	}
	else {
		Server_SetWantTrade(NewValue);
	}
}

void AC_InventoryCharacter::Server_SetWantTrade_Implementation(bool NewValue)
{
	WantTrade = NewValue;
}

void AC_InventoryCharacter::TryTrade(AC_InventoryCharacter* Character)
{
	if (WantTrade && Character->WantTrade) {
		OnTrade(Character);
		Character->OnTrade(this);

	}
	else {
		/*Server*/
		if (HasAuthority()) {
			Multicast_TryTrade(Character);
		}
		/*Client*/
		else {
			Server_TryTrade(Character);
		}
	}
}


bool AC_InventoryCharacter::Multicast_TryTrade_Validate(AC_InventoryCharacter* Character)
{
	return true;
}

void AC_InventoryCharacter::Multicast_TryTrade_Implementation(AC_InventoryCharacter* Character)
{

	if (IsLocallyControlled()) {
		if (HUDWidget) {
			HUDWidget->TradeRequest(Character);
		}
	}
}

bool AC_InventoryCharacter::Server_TryTrade_Validate(AC_InventoryCharacter* Character)
{
	return true;
}


void AC_InventoryCharacter::Server_TryTrade_Implementation(AC_InventoryCharacter* Character)
{
	Multicast_TryTrade(Character);
}






