// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyHUD.h"
#include "Lobby/Widget/LobbyHUDWidget.h"
#include "Kismet/GameplayStatics.h"


void ALobbyHUD::BeginPlay()
{
	HUDWidget = Cast<ULobbyHUDWidget>(CreateWidget(GetWorld(), Widgetclass));

	if (HUDWidget) {
		HUDWidget->AddToViewport();

		if(APlayerController* Controller = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))){
			Controller->SetInputMode(FInputModeUIOnly());
			Controller->SetShowMouseCursor(true);
		}
	}
}
