// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Item.h"
#include "Food.generated.h"


class AC_InventoryCharacter;

/**
 * 
 */
UCLASS()
class C_INVENTORY_API AFood : public AItem
{
	GENERATED_BODY()

public:
	AFood();
	

protected:

	UPROPERTY(EditAnywhere)
	float RemoveFoodValue;



public:
	virtual void Use(AC_InventoryCharacter* Character, bool IsInShop =false, bool IsInTradeWIdget = false) override;


};
