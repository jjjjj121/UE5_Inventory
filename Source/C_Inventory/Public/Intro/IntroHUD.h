// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "OnlineSessionSettings.h"
#include "IntroHUD.generated.h"

/**
 * 
 */
UCLASS()
class C_INVENTORY_API AIntroHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> Widgetclass;

	class UIntro_HUDWidget* HUDWidget;

protected:
	virtual void BeginPlay() override;

public:
	void FindSessionComplete(TArray<FOnlineSessionSearchResult> SessionResults);

};
