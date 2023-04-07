// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "hudwidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class C_INVENTORY_API Uhudwidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "Hudwidget", meta = (BindWidget))
	UTextBlock* TB_Hunger;

	UPROPERTY(BlueprintReadOnly, Category = "Hudwidget", meta = (BindWidget))
	UTextBlock* TB_Health;

public:

	UTextBlock* GetTB_Hunger();
	UTextBlock* GetTB_Health();

public:
	UFUNCTION(BlueprintCallable, Category = "HudWidget")
	void UpdateStats(float Hunger, float Health);
};
