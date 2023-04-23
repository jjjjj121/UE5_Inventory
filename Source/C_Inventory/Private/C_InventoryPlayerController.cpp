// Fill out your copyright notice in the Description page of Project Settings.


#include "C_InventoryPlayerController.h"
#include "Gameframework/PlayerState.h"
#include "C_Inventory/C_InventoryCharacter.h"


void AC_InventoryPlayerController::OnPossess(APawn* aPawn)
{
    Super::OnPossess(aPawn);

    if (AC_InventoryCharacter* MyCharacter = Cast<AC_InventoryCharacter>(aPawn)) {
        MyCharacter->Nickname = PlayerState->GetPlayerName();
    }

}


