// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Medpack.h"
#include "C_Inventory/C_InventoryCharacter.h"


AMedpack::AMedpack()
{
	HealthValue = 10.0f;
	ItemData.ItemClass = StaticClass();
}

void AMedpack::Use(AC_InventoryCharacter* Character)
{
	if (Character) {
		Character->AddHealth(HealthValue);
	}
	
}
