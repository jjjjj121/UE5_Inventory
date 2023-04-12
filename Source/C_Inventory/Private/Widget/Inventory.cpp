// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory.h"
#include "C_Inventory/Public/Widget/InventorySlot.h"
#include "Components/UniformGridPanel.h"
#include "C_Inventory/Public/Actors/Item.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "C_Inventory/C_InventoryCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "C_Inventory/Public/Actors/Gold.h"
#include "C_Inventory/Public/Widget/switchingwidget.h"

void UInventory::NativeConstruct()
{
	Super::NativeConstruct();

	BT_Gold->OnClicked.AddDynamic(this, &UInventory::OnClickGold);
	ETB_Gold->OnTextCommitted.AddDynamic(this, &UInventory::OnTextCommit);
	InitInventory();
}

void UInventory::OnClickGold()
{
	
	if (AC_InventoryCharacter* Character = Cast<AC_InventoryCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))) {
		if (Character->bRunningTrade) {
			ETB_Gold->SetVisibility(ESlateVisibility::Visible);
			if (APlayerController* Controller = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))) {
				ETB_Gold->SetUserFocus(Controller);
			}
		}
	}
}



void UInventory::OnTextCommit(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter) {
		if (AC_InventoryCharacter* Character = Cast<AC_InventoryCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))) {
			int32 TradeValue = FCString::Atof(*Text.ToString());
			Character->GetGold() > TradeValue ? TradeValue : TradeValue = Character->GetGold();

			FItemData GoldData;
			GoldData.ItemClass = AGold::StaticClass();
			GoldData.StackCount = TradeValue;

			Character->UpdateGold(-TradeValue, false);
			Character->UpdateGold(TradeValue, true);
			if (ParentWidget->TradeCharacter->HasAuthority()) {
				ParentWidget->TradeCharacter->SetUserTradeGold(Character->GetTradeGold());
			}
			else {
				Character->ClientSetUserTradeGold(Character->GetTradeGold() + TradeValue);
			}

			ETB_Gold->SetText(FText::FromString(FString::Printf(TEXT(""))));
			ETB_Gold->SetVisibility(ESlateVisibility::Collapsed);

		}
	}

}

UUniformGridPanel* UInventory::GetGrid_Inventory()
{
	return Grid_Inventory;
}

UTextBlock* UInventory::GetTB_Gold()
{
	return TB_Gold;
}

UButton* UInventory::GetBT_Gold()
{
	return BT_Gold;
}

UEditableTextBox* UInventory::GetETB_Gold()
{
	return ETB_Gold;
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


