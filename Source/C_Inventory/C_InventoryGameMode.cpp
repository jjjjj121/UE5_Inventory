// Copyright Epic Games, Inc. All Rights Reserved.

#include "C_InventoryGameMode.h"
#include "C_InventoryCharacter.h"
#include "C_InventoryPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AC_InventoryGameMode::AC_InventoryGameMode()
{
	PlayerControllerClass = AC_InventoryPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

}
