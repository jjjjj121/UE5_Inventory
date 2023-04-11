// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/TradeWidget.h"
#include "C_Inventory/Public/Widget/InventorySlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "C_Inventory/C_InventoryCharacter.h"
#include "C_Inventory/Public/Widget/switchingwidget.h"
#include "C_Inventory/Public/Actors/Item.h"
#include "Components/TextBlock.h"

UUniformGridPanel* UTradeWidget::GetGrid_UserTrade()
{
    return Grid_UserTrade;
}

UUniformGridPanel* UTradeWidget::GetGrid_MyTrade()
{
    return Grid_MyTrade;
}

UTextBlock* UTradeWidget::GetTB_UserTradeGold()
{
    return TB_UserTradeGold;
}

UTextBlock* UTradeWidget::GetTB_MyTradeGold()
{
    return TB_MyTradeGold;
}

UButton* UTradeWidget::GetBT_Close()
{
    return BT_Close;
}

void UTradeWidget::NativeConstruct()
{
    Super::NativeConstruct();
	BT_Close->OnClicked.AddDynamic(this, &UTradeWidget::CloseWidget);
    InitInventory();

}

void UTradeWidget::InitInventory()
{
	for (int i = 0; i < AmountofGrid; ++i) {
		if (SlotWidgetClass) {
			UInventorySlot* UserTradeSlot = Cast<UInventorySlot>(CreateWidget(GetWorld(), SlotWidgetClass));
			UInventorySlot* MyTradeSlot = Cast<UInventorySlot>(CreateWidget(GetWorld(), SlotWidgetClass));
			if (UserTradeSlot && MyTradeSlot) {
				UserTradeSlot->TradeWidget = this, MyTradeSlot->TradeWidget = this;
				UserTradeSlot->IsUserTradeItem = true; MyTradeSlot->IsMyTradeItem = true;
				
				Grid_UserTrade->AddChildToUniformGrid(UserTradeSlot, CurrentRow, CurrentColumn);
				Grid_MyTrade->AddChildToUniformGrid(MyTradeSlot, CurrentRow, CurrentColumn);
				++CurrentColumn;
				if (CurrentColumn >= 7) {
					CurrentColumn = 0;
					++CurrentRow;
				}
			}
		}
	}

	CurrentColumn = 0, CurrentRow = 0;
}

void UTradeWidget::AddItem(FItemData ItemData)
{
	UE_LOG(LogTemp, Warning, TEXT("Trade Widget Add Item"));
	auto AllChildren = Grid_MyTrade->GetAllChildren();
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

void UTradeWidget::Update(TArray<FItemData> Items)
{
	if (Items.Num() < TotalInventoryNum) {
		RemoveItem(Items);
	}
	else {
		for (FItemData& Item : Items) {
			auto AllChildren = Grid_MyTrade->GetAllChildren();
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

void UTradeWidget::RemoveItem(TArray<FItemData> Items)
{
	auto AllChildren = Grid_MyTrade->GetAllChildren();

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

bool UTradeWidget::IsNewItem(TArray<FItemData> Items)
{
	return Items.Num() > TotalInventoryNum;
}

void UTradeWidget::UpdateGold(int32 Gold)
{
	UE_LOG(LogTemp, Warning, TEXT("UPDATE TRADE GOLD"));
	TradeGold = Gold;
	TB_MyTradeGold->SetText(FText::FromString(FString::Printf(TEXT("%d"), TradeGold)));
}

void UTradeWidget::CloseWidget()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
	if (ParentWidget) {
		ParentWidget->SwitchingUI(false);
	}
	if (AC_InventoryCharacter* OwnCharacter = Cast<AC_InventoryCharacter>(this->GetOwningPlayer()->GetCharacter())) {
		OwnCharacter->SetWantTrade(false);
		if (TradeCharacter->HasAuthority()) {
			TradeCharacter->EndTrade();
		}
		else {
			OwnCharacter->EndTrade(TradeCharacter);
		}
	}
	

}


