// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractInterface.h"
#include "C_Inventory/Public/Structs.h"
#include "Item.h"
#include "ShopKeeper.generated.h"

UCLASS()
class C_INVENTORY_API AShopKeeper : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShopKeeper();

protected:
	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* ShopKeeperMesh;

	UPROPERTY(ReplicatedUsing = OnRep_Items, EditAnywhere, BlueprintReadWrite, Category = "Property")
	TArray<FItemData> Items;

	UFUNCTION()
	void OnRep_Items();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Interact(class AC_InventoryCharacter* Character) override;

	virtual void TransfferedItem(TSubclassOf<AItem> ItemSubclass);
};
