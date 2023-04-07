// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_Inventory/Public/Structs.h"
#include "Inventory.generated.h"

class UUniformGridPanel;
class UTextBlock;
/**
 * 
 */
UCLASS()
class C_INVENTORY_API UInventory : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "Inventory", meta = (BindWidget))
	UUniformGridPanel* Grid_Inventory;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory", meta = (BindWidget))
	UTextBlock* TB_Gold;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> SlotWidgetClass;

public:
	
	UUniformGridPanel* GetGrid_Inventory();
	UTextBlock* GetTB_Gold();


public:
	void InitInventory();
	void AddItem(FItemData ItemData);
	void Update(TArray<FItemData> Items);
	void RemoveItem(TArray<FItemData> Items);
	bool IsNewItem(TArray<FItemData> Items);
	void UpdateGold(int32 Gold);

protected:
	int32 CurrentRow =0;
	int32 CurrentColumn =0;
	int32 TotalInventoryNum = 0;
	int32 AmountofGrid = 20;

protected:
	virtual void NativeConstruct() override;

};
