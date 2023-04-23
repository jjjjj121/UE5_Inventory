// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "C_InventoryPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class C_INVENTORY_API AC_InventoryPlayerController : public APlayerController
{
	GENERATED_BODY()
	

private:
	virtual void OnPossess(APawn* aPawn) override;

};
