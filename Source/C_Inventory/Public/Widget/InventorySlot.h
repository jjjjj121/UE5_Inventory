// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_Inventory/Public/Structs.h"
#include "InventorySlot.generated.h"

class UTextBlock;
class UImage;
class AShopKeeper;
class UDragItemVisual;

/**
 * 
 */
UCLASS()
class C_INVENTORY_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UPROPERTY(BlueprintReadOnly, Category = "InventorySlot", meta = (BindWidget))
	UTextBlock* TB_Stack;

	UPROPERTY(BlueprintReadOnly, Category = "InventorySlot", meta = (BindWidget))
	UImage* Img_Item;

	UPROPERTY(BlueprintReadWrite, Category = "InventorySlot")
	FItemData ItemData;

	UPROPERTY(EditDefaultsOnly, Category = "InventorySlot")
	TSubclassOf<UDragItemVisual> DragItemVisualclass;



public:
	AShopKeeper* ShopKeeper;
	bool IsShopItem = false;

public:

	UTextBlock* GetItemText();
	UImage* GetItemImage();

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	

public:
	void UpdateItem(FItemData NewItemData);
	void EnableButton();
	void DisableButton();


public:
	/*Test*/
	UFUNCTION(blueprintimplementableevent)
	void Test();

};
