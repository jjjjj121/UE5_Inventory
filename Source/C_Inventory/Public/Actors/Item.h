// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	

public:
	virtual void Interact() override;

};
