// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "C_InventoryCharacter.generated.h"

class AItem;

UCLASS(config = Game)
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
		float Health = 100.0f;

	UPROPERTY(ReplicatedUsing = OnRep_Stats, BlueprintReadWrite, Category = "Property")
		float Hunger = 100.0f;

	UPROPERTY(ReplicatedUsing = OnRep_InventoryItems, BlueprintReadOnly, Category = "Property")
		int32 MyGold = 0;

	UPROPERTY(ReplicatedUsing = OnRep_TradeItems, BlueprintReadOnly, Category = "Property")
		int32 TradeGold = 0;

	UFUNCTION()
		void OnRep_Stats();


	/*Notify replicate를 실행하는 변수 선언*/
	UPROPERTY(ReplicatedUsing = OnRep_InventoryItems, BlueprintReadWrite, Category = "Property")
		TArray<FItemData> InventoryItems;
	/*위 변수가 업데이트 될 때마다 호출될 Notify 함수 -> void OnRep_FUNCNAME();이 기본 형식*/
	UFUNCTION()
		void OnRep_InventoryItems();

	UPROPERTY(ReplicatedUsing = OnRep_TradeItems, BlueprintReadWrite, Category = "Property")
		TArray<FItemData> TradeItems;

	UFUNCTION()
		void OnRep_TradeItems();

	void User_TradeItem(const TArray<FItemData>& NewTradeWidgetItems, bool IsMyTradeSlot = false);

	UFUNCTION(NetMulticast, Reliable)
		void Multi_User_TradeItems(const TArray<FItemData>& NewTradeWidgetItems, bool IsMyTradeSlot = false);

	UFUNCTION(Server, Reliable)
		void Server_User_TradeItems(const TArray<FItemData>& NewTradeWidgetItems, bool IsMyTradeSlot = false);

	void AddItemAndUpdateTradeWidget(FItemData ItemData, const TArray<FItemData>& NewTradeWidgetItems = TArray<FItemData>(), bool IsMyTradeSlot = true);

public:

	/*아이템 사용 함수 -> 인벤토리 아이콘 클릭 시 호출*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void UseItem(TSubclassOf<AItem> ItemSubclass, AShopKeeper* ShopKeeper, bool IsShopItem = false, bool IsTradeWidgetItem = false);

	/*UseItem RPC함수 : client -> server */
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_UseItem(TSubclassOf<AItem> ItemSubclass, AShopKeeper* ShopKeeper, bool IsShopItem = false, bool IsTradeWidgetItem = false);


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

	UFUNCTION(BlueprintNativeEvent, Category = "InventorySystem")
		void AddItemAndUpdateInventory(FItemData ItemData, const TArray<FItemData>& NewInventoryItems = TArray<FItemData>());

	UFUNCTION(BlueprintNativeEvent, Category = "InventorySystem")
		void OpenShop(const TArray<FItemData>& Items, AShopKeeper* ShopKeeper);

	UFUNCTION(BlueprintNativeEvent, Category = "InventorySystem")
		void UpdateShop(const TArray<FItemData>& Items);

	UFUNCTION(BlueprintCallable)
		int32 GetGold();

	UFUNCTION(BlueprintCallable)
		int32 GetTradeGold();

	UFUNCTION(Server, Reliable)
		void Server_UpdateGold(int32 GoldValue, bool IsMyGold);


public:
	/*Test Widget*/
	UPROPERTY(EditAnywhere, Category = "Widget")
		TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "Widget")
		class Uswitchingwidget* HUDWidget;

	UFUNCTION(BlueprintCallable, Category = "Widget")
		void SwitchingUI();


public:

	void AddInventoryItem(FItemData ItemData, bool IsInventoryItem = true);
	void AddHealth(float Value);
	void RemoveHunger(float Value);

	void UpdateGold(int32 GoldValue, bool IsMyGold);

	void TryTrade();
	void TryTrade(AC_InventoryCharacter* Character);
	void UserTryTrade();
	void OnTrade(AC_InventoryCharacter* TradeUser);

	void EndTrade();
	void EndTrade(TArray<FItemData> SucceedTradeItems);
	void UserEndTrade();

	void SetWantTrade(bool NewValue);
	void SetRunningTrade(bool NewValue);

	void SetUserTradeGold(int32 GoldValue);
	void UserSetUserTradeGold(int32 GoldValue);

	void SetAcceptTrade();
	void UserSetAcceptTrade();

	void SucceedTrade();
	void SucceedTrade(TArray<FItemData> SucceedTradeItems);
	void UserSucceedTrade();

	void TradeReset();
	void UserTradeReset();
public:

	UFUNCTION(Server, Reliable)
		void Server_TryTrade(AC_InventoryCharacter* TradeUser);

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_TryTrade(AC_InventoryCharacter* TradeUser);

	UFUNCTION(Server, Reliable)
		void Server_UserTryTrade(AC_InventoryCharacter* TradeUser);

	UFUNCTION(Server, Reliable)
		void Server_SetWantTrade(bool NewValue);

	UFUNCTION(Server, Reliable)
		void Server_SetRunningTrade(bool NewValue);

	UFUNCTION(Client, Reliable, WithValidation)
		void Client_OnTrade(AC_InventoryCharacter* TradeUser);

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_EndTrade();

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_SetUserTradeGold(int32 GoldValue);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_AddInventoryItem(FItemData ItemData, bool IsInventoryItem = true);

	UFUNCTION(Server, Reliable)
		void Server_UserSetUserTradeGold(AC_InventoryCharacter* NewTradeCharacter, int32 GoldValue);

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_SetAcceptTrade();

	UFUNCTION(Server, Reliable)
		void Server_UserSetAcceptTrade(AC_InventoryCharacter* NewTradeCharacter);

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_SucceedTrade();

	UFUNCTION(Server, Reliable)
		void Server_UserSucceedTrade(AC_InventoryCharacter* NewTradeCharacter);

	UFUNCTION(Server, Reliable)
		void Server_TradeReset();

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_TradeReset();

	UFUNCTION(Server, Reliable)
		void Server_UserTradeReset(AC_InventoryCharacter* NewTradeCharacter);

	UFUNCTION(Server, Reliable)
		void Server_UserEndTrade(AC_InventoryCharacter* NewTradeCharacter);



private:
	/*ID 만들 경우*/
	//UPROPERTY(BlueprintReadOnly)
	//FString UserID;


public:
	UPROPERTY(Replicated, BlueprintReadOnly)
		bool WantTrade;

	UPROPERTY(Replicated, BlueprintReadOnly)
		bool bRunningTrade = false;

	AC_InventoryCharacter* TradeCharacter;

};

