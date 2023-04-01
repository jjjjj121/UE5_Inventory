// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "C_InventoryCharacter.generated.h"

class AItem;

UCLASS(config=Game)
class AC_InventoryCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/* Interact Input Action*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;


public:
	AC_InventoryCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Interact();
	void Interact(FVector Start, FVector End);

	/*Interact RPC함수 : client -> server */
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Interact(FVector Start, FVector End);
	

	/*Stats updates Function*/
	UFUNCTION(BlueprintNativeEvent)
	void UpdateStats(float NewHunger, float NewHealth);
	/*아래 함수도 선언을 해줘야하는건지 정확히 모르겠음*/
	//void UpdateStats_Implementation(float NewHunger, float NewHealth);

protected:

	UPROPERTY(ReplicatedUsing = OnRep_Stats, BlueprintReadWrite, Category = "Property")
	float Health;

	UPROPERTY(ReplicatedUsing = OnRep_Stats, BlueprintReadWrite, Category = "Property")
	float Hunger;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Property")
	int MyGold = 0;

	UFUNCTION()
	void OnRep_Stats();


	/*Notify replicate를 실행하는 변수 선언*/
	UPROPERTY(ReplicatedUsing = OnRep_InventoryItems, BlueprintReadWrite, Category = "Property")
	TArray<FItemData> InventoryItems;
	/*위 변수가 업데이트 될 때마다 호출될 Notify 함수 -> void OnRep_FUNCNAME();이 기본 형식*/
	UFUNCTION()
	void OnRep_InventoryItems();
	
	/*아이템 - 인벤토리에 추가 함수*/
	/*BlueprintImplementableEvent 키워드는 특수상황에 대한 사용자이벤트를 만들기 위한 키워드로 헤더에만 선언하고 본문은 블루프린트에서 작성한다. */
	UFUNCTION(BlueprintImplementableEvent, Category = "InventorySystem")
	void AddItemAndUpdateInventory(FItemData ItemData, const TArray<FItemData>& NewInventoryItems = TArray<FItemData>());

	/*아이템 사용 함수 -> 인벤토리 아이콘 클릭 시 호출*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UseItem(TSubclassOf<AItem> ItemSubclass, AShopKeeper* ShopKeeper ,bool IsShopItem = false);

	/*UseItem RPC함수 : client -> server */
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_UseItem(TSubclassOf<AItem> ItemSubclass, AShopKeeper* ShopKeeper ,bool IsShopItem =false);

			
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	/*Replicate 속성을 사용하기 위해 속성값을 등록해주기 위한 함수*/
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintImplementableEvent)
	void OpenShop(const TArray<FItemData>& Items, AShopKeeper* ShopKeeper);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateShop(const TArray<FItemData>& Items);

public:

	void AddInventoryItem(FItemData ItemData);
	void AddHealth(float Value);
	void RemoveHunger(float Value);

};

