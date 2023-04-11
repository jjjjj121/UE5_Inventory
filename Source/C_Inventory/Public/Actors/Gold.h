// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Item.h"
#include "Gold.generated.h"

/**
 * 
 */
UCLASS()
class C_INVENTORY_API AGold : public AItem
{
	GENERATED_BODY()
	
public:
	AGold();

	virtual void Use(AC_InventoryCharacter* Character, bool IsInShop = false, bool IsInTradeWIdget = false) override;
};
