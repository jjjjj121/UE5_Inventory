// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyHUDWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class C_INVENTORY_API ULobbyHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "Lobby HUD Widget", meta = (BindWidget))
		UButton* BT_GameStart;

public:
	UButton* GetBT_GameStart();

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void OnClickedGameStart();

};
