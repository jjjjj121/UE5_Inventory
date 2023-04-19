// Fill out your copyright notice in the Description page of Project Settings.


#include "Intro/IntroHUD.h"
#include "Intro/Widget/Intro_HUDWidget.h"
#include "Kismet/GameplayStatics.h"

void AIntroHUD::BeginPlay()
{
	Super::BeginPlay();

	HUDWidget = Cast<UIntro_HUDWidget>(CreateWidget(GetWorld(), Widgetclass));

	if (HUDWidget) {
		HUDWidget->AddToViewport();
		if (APlayerController* Controller = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))) {
			Controller->SetInputMode(FInputModeUIOnly());
			Controller->SetShowMouseCursor(true);
		}
	}

}

void AIntroHUD::FindSessionComplete(TArray<FOnlineSessionSearchResult> SessionResults)
{
	HUDWidget->FindSessionComplete(SessionResults);
}
