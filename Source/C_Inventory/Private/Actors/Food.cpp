// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Food.h"
#include "C_Inventory/C_InventoryCharacter.h"

AFood::AFood()
{
	RemoveFoodValue = 30.0f;
	ItemData.ItemClass = StaticClass();
}

void AFood::Use(AC_InventoryCharacter* Character, bool IsInShop)
{
	Super::Use(Character, IsInShop);

	if (Character && !IsInShop) {
		Character->RemoveHunger(RemoveFoodValue);
	}
	
}
