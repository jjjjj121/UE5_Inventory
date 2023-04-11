// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Medpack.h"
#include "C_Inventory/C_InventoryCharacter.h"


AMedpack::AMedpack()
{
	HealthValue = 10.0f;
	ItemData.ItemClass = StaticClass();
}

void AMedpack::Use(AC_InventoryCharacter* Character, bool IsInShop, bool IsInTradeWIdget)
{
	Super::Use(Character, IsInShop);

	if (Character) {
		if (!Character->bRunningTrade && !IsInShop) {
			Character->AddHealth(HealthValue);
		}
	}


}
