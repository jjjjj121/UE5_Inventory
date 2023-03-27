// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Item.h"
#include "Medpack.generated.h"



class AC_InventoryCharacter;

/**
 * 
 */
UCLASS()
class C_INVENTORY_API AMedpack : public AItem
{
	GENERATED_BODY()
	
public:
	AMedpack();


protected:

	UPROPERTY(EditAnywhere)
	float HealthValue;



public:
	virtual void Use(AC_InventoryCharacter* Character) override;





};
