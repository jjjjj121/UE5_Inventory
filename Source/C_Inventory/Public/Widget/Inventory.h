// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_Inventory/Public/Structs.h"
#include "Inventory.generated.h"

class UUniformGridPanel;
class UTextBlock;
class UButton;
class UEditableTextBox;
class Uswitchingwidget;
class AC_InventoryCharacter;
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

	UPROPERTY(BlueprintReadOnly, Category = "Inventory", meta = (BindWidget))
	UButton* BT_Gold;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory", meta = (BindWidget))
	UEditableTextBox* ETB_Gold;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> SlotWidgetClass;

	Uswitchingwidget* ParentWidget;
public:
	
	UUniformGridPanel* GetGrid_Inventory();
	UTextBlock* GetTB_Gold();
	UButton* GetBT_Gold();
	UEditableTextBox* GetETB_Gold();
	int32 GetTotalInventoryNum();

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

	UFUNCTION()
	void OnClickGold();

	UFUNCTION()
	void OnTextCommit(const FText& Text, ETextCommit::Type CommitMethod);


};
