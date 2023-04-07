// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_Inventory/Public/Structs.h"
#include "Shop.generated.h"


class UUniformGridPanel;
class UTextBlock;
class UButton;
class AShopKeeper;

/**
 * 
 */
UCLASS()
class C_INVENTORY_API UShop : public UUserWidget
{
	GENERATED_BODY()
	

public:

	UPROPERTY(BlueprintReadOnly, Category = "Shop", meta = (BindWidget))
	UUniformGridPanel* Grid_Shop;

	UPROPERTY(BlueprintReadOnly, Category = "Shop", meta = (BindWidget))
	UButton* BT_Close;
	
	UPROPERTY(EditAnywhere, Category = "Shop")
	TSubclassOf<UUserWidget> SlotWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "Shop")
	class Uswitchingwidget* ParentWidget;

public:

	UUniformGridPanel* GetGrid_Shop();
	UButton* GetBT_Close();


protected:
	int32 CurrentRow = 0;
	int32 CurrentColumn = 0;
	int32 TotalInventoryNum = 0;
	int32 AmountofGrid = 20;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void CloseButtonClick();


public:
	void InitInventory();
	void AddShopItem(TArray<FItemData> Items, AShopKeeper* NewShopKeeper);
	void ClearShopItem();
	void UpdateShop(TArray<FItemData> Items);
	void RemoveShopItem(TArray<FItemData> Items);


};
