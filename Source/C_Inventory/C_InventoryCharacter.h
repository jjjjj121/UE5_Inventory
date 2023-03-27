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


	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Interact(FVector Start, FVector End);
	

	/*Stats updates Function*/
	UFUNCTION(BlueprintNativeEvent)
	void UpdateStats(float NewHunger, float NewHealth);
	/*�Ʒ� �Լ��� ������ ������ϴ°��� ��Ȯ�� �𸣰���*/
	//void UpdateStats_Implementation(float NewHunger, float NewHealth);

protected:

	UPROPERTY(BlueprintReadWrite, Category = "Property")
	float Health;

	UPROPERTY(BlueprintReadWrite, Category = "Property")
	float Hunger;

	/*Notify replicate�� �����ϴ� ���� ����*/
	UPROPERTY(ReplicatedUsing = OnRep_InventoryItems, BlueprintReadWrite, Category = "Property")
	TArray<FItemData> InventoryItems;
	/*�� ������ ������Ʈ �� ������ ȣ��� Notify �Լ� -> void OnRep_FUNCNAME();�� �⺻ ����*/
	UFUNCTION()
	void OnRep_InventoryItems();
	
	/*������ - �κ��丮�� �߰� �Լ�*/
	/*BlueprintImplementableEvent Ű����� Ư����Ȳ�� ���� ������̺�Ʈ�� ����� ���� Ű����� ������� �����ϰ� ������ �������Ʈ���� �ۼ��Ѵ�. */
	UFUNCTION(BlueprintImplementableEvent, Category = "InventorySystem")
	void AddItemToInventory(FItemData ItemData);


	/*������ ��� �Լ� -> �κ��丮 ������ Ŭ�� �� ȣ��*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UseItem(TSubclassOf<AItem> ItemSubclass);
			
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	/*Replicate �Ӽ��� ����ϱ� ���� �Ӽ����� ������ֱ� ���� �Լ�*/
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:

	void AddInventoryItem(FItemData ItemData);
	void AddHealth(float Value);
	void RemoveHunger(float Value);

};

