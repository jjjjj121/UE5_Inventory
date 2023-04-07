// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "switchingwidget.generated.h"

class UInventory;
class Uhudwidget;
class UWidgetSwitcher;
class UShop;
/**
 * 
 */
UCLASS()
class C_INVENTORY_API Uswitchingwidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "Inventory Widget", meta = (BindWidget))
	UInventory* W_Inventory;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Widget", meta = (BindWidget))
	Uhudwidget* W_HUD;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Widget", meta = (BindWidget))
	UWidgetSwitcher* WS_Interface;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Widget", meta = (BindWidget))
	UShop* W_Shop;

public:

	UInventory* GetW_Inventory();
	Uhudwidget* GetW_HUD();
	UWidgetSwitcher* GetWS_Interface();
	UShop* GetW_Shop();
	int32 GetActiveWidgetIndex();


public:
	UFUNCTION(BlueprintCallable)
	void SwitchingUI(bool IsInventoryUI);
	void UpdateStats(float Hunger, float Health);
	void OpenShopUI();

protected:
	virtual void NativeConstruct() override;

};
