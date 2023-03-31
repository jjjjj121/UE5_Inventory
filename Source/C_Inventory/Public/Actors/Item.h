// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_Inventory/Public/Structs.h"
#include "C_Inventory/Public/InteractInterface.h"
#include "Item.generated.h"

UCLASS()
class C_INVENTORY_API AItem : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	class UStaticMeshComponent* ItemMesh;
	
	UPROPERTY(EditAnywhere, Category = "Item")
	FItemData ItemData;

public:
	virtual void Interact(class AC_InventoryCharacter* Character) override;

	virtual void Use(AC_InventoryCharacter* Character, bool IsInShop = false) override;

	

public:
	/*ItemData를 받아오기 위한 함수*/
	FItemData GetItemData() const { return ItemData; }

	//FString TestFunction() const { return FString("OUR TEST STRING"); }

};
