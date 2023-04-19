// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/Widget/LobbyHUDWidget.h"
#include "Components/Button.h"
#include "C_Inventory/C_InventoryCharacter.h"
#include "Kismet/GameplayStatics.h"

UButton* ULobbyHUDWidget::GetBT_GameStart()
{
	return BT_GameStart;
}

void ULobbyHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BT_GameStart->OnClicked.AddDynamic(this, &ULobbyHUDWidget::OnClickedGameStart);

	if (APlayerController* Controller = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))) {
		if (Controller->HasAuthority()) {
			BT_GameStart->SetVisibility(ESlateVisibility::Visible);
		}
		else {
			BT_GameStart->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	

}

void ULobbyHUDWidget::OnClickedGameStart()
{
	if (APlayerController* Controller = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))) {
		if (Controller->HasAuthority()) {
			GetWorld()->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen'");
		}
	}
	
}
