// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory.h"
#include "C_Inventory/Public/Widget/InventorySlot.h"
#include "Components/UniformGridPanel.h"
#include "C_Inventory/Public/Actors/Item.h"
#include "Components/TextBlock.h"

void UInventory::NativeConstruct()
{
	Super::NativeConstruct();
	InitInventory();
}

UUniformGridPanel* UInventory::GetGrid_Inventory()
{
	return Grid_Inventory;
}

UTextBlock* UInventory::GetTB_Gold()
{
	return TB_Gold;
}

void UInventory::InitInventory()
{

	for (int i = 0; i < AmountofGrid; ++i) {
		if (SlotWidgetClass) {
			if (UInventorySlot* InventorySlot = Cast<UInventorySlot>(CreateWidget(GetWorld(), SlotWidgetClass))) {
				Grid_Inventory->AddChildToUniformGrid(InventorySlot, CurrentRow, CurrentColumn);
				++CurrentColumn;
				if (CurrentColumn >= 4) {
					CurrentColumn = 0;
					++CurrentRow;
				}
			}
		}
	}

	CurrentColumn = 0, CurrentRow = 0;

}

void UInventory::AddItem(FItemData ItemData)
{

	auto AllChildren = Grid_Inventory->GetAllChildren();
	for (auto Child : AllChildren) {
		if (UInventorySlot* InventorySlot = Cast<UInventorySlot>(Child)) {
			if (!InventorySlot->ItemData.ItemClass) {
				UE_LOG(LogTemp, Warning, TEXT("ItemClass Is Null"));
				InventorySlot->UpdateItem(ItemData);
				++TotalInventoryNum;
				break;
			}
		}
	}

}

void UInventory::Update(TArray<FItemData> Items)
{
	if (Items.Num() < TotalInventoryNum) {
		RemoveItem(Items);
	}
	else {
		for (FItemData& Item : Items) {
			auto AllChildren = Grid_Inventory->GetAllChildren();
			for (auto Child : AllChildren) {
				if (UInventorySlot* InventorySlot = Cast<UInventorySlot>(Child)) {
					if (InventorySlot->ItemData.ItemClass == Item.ItemClass) {
						InventorySlot->UpdateItem(Item);
						break;
					}
				}
			}
		}
	}

}

void UInventory::RemoveItem(TArray<FItemData> Items)
{
	auto AllChildren = Grid_Inventory->GetAllChildren();

	/*check all inventory slot*/
	for (auto Child : AllChildren) {
		if (UInventorySlot* InventorySlot = Cast<UInventorySlot>(Child)) {
			/*ItemData가 존재하는 Slot*/
			if (InventorySlot->ItemData.ItemClass) {
				if (Items.Num()) {
					for (FItemData& Item : Items) {
						/*해당 슬롯에 등록된 데이터와 새로운 데이터가 다른 경우 -> 소멸되야할 아이템*/
						if (Item.ItemClass != InventorySlot->ItemData.ItemClass) {
							InventorySlot->DisableButton();
						}
					}
				}
				else {
					InventorySlot->DisableButton();
				}
			}
		}
	}


	--TotalInventoryNum;
}

bool UInventory::IsNewItem(TArray<FItemData> Items)
{
	return Items.Num() > TotalInventoryNum;
}

void UInventory::UpdateGold(int32 GoldValue)
{
	TB_Gold->SetText(FText::FromString(FString::Printf(TEXT("%d"), GoldValue)));
}


