// Fill out your copyright notice in the Description page of Project Settings.



#include "Widget/InventorySlot.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "C_Inventory/C_InventoryCharacter.h"
#include "C_Inventory/Public/Widget/DragItemVisual.h"
#include "C_Inventory/Public/Widget/SlotDragOperation.h"
#include "C_Inventory/Public/Actors/Item.h"


UTextBlock* UInventorySlot::GetItemText()
{
	return TB_Stack;
}

UImage* UInventorySlot::GetItemImage()
{
	return Img_Item;
}

void UInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();
}

FReply UInventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
		//UE_LOG(LogTemp, Warning, TEXT("LEFT MOUSE BUTTON! : %d"), IsShopItem);
		/*드레그 했는지 감지*/
		Reply.DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}

	return Reply;
}

FReply UInventorySlot::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
		if (AC_InventoryCharacter* PlayerCharacter = Cast<AC_InventoryCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))) {
			PlayerCharacter->UseItem(ItemData.ItemClass, ShopKeeper, IsShopItem, IsMyTradeItem);
		}
	}


	return Reply;
}


void UInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{

	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UE_LOG(LogTemp, Warning, TEXT("On Drag Detected"));

	if (DragItemVisualclass) {
		if (UDragItemVisual* DragVisual = CreateWidget<UDragItemVisual>(this, DragItemVisualclass)) {
			DragVisual->Img_DragItem->SetBrushFromTexture(ItemData.ItemImage);


			USlotDragOperation* DragItemOperation = NewObject<USlotDragOperation>();
			/*Payload : 전달하려는 데이터의 UObject*/
			DragItemOperation->Payload = this;
			/*Default Drag Visual : 아이템을 Drag 할 때 화면에 표시할 위젯*/
			DragItemOperation->DefaultDragVisual = DragVisual;
			/*Pivot : Drag Visual의 피벗 위치 설정*/
			DragItemOperation->Pivot = EDragPivot::CenterCenter;

			OutOperation = DragItemOperation;
		}

	}

}

bool UInventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	UE_LOG(LogTemp, Warning, TEXT("On Drop!"));

	USlotDragOperation* DropItemOperation = Cast<USlotDragOperation>(InOperation);
	if (DropItemOperation) {

		UInventorySlot* DroppedItem = Cast<UInventorySlot>(DropItemOperation->Payload);
		if (DroppedItem) {

			FItemData TempData = DroppedItem->ItemData;
			DroppedItem->UpdateItem(ItemData);
			UpdateItem(TempData);

		}

	}

	return false;
}



void UInventorySlot::UpdateItem(FItemData NewItemData)
{
	ItemData = NewItemData;
	UE_LOG(LogTemp, Warning, TEXT("StackCount : %d"), ItemData.StackCount);
	if (ItemData.ItemClass) {
		if (ItemData.StackCount > 0) {
			EnableButton();
			TB_Stack->SetText(FText::FromString(FString::Printf(TEXT("%d"), ItemData.StackCount)));
			return;
		}
	}

	DisableButton();

}

void UInventorySlot::EnableButton()
{
	Img_Item->SetBrushFromTexture(ItemData.ItemImage);
	Img_Item->SetOpacity(1);

}

void UInventorySlot::DisableButton()
{
	Img_Item->SetOpacity(0);
	TB_Stack->SetText(FText());
	ItemData.SetDefault();

}

