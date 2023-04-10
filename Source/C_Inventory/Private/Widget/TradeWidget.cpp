// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/TradeWidget.h"
#include "C_Inventory/Public/Widget/InventorySlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "C_Inventory/C_InventoryCharacter.h"
#include "C_Inventory/Public/Widget/switchingwidget.h"

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
}

void UTradeWidget::Update(TArray<FItemData> Items)
{
}

void UTradeWidget::RemoveItem(TArray<FItemData> Items)
{
}

bool UTradeWidget::IsNewItem(TArray<FItemData> Items)
{
    return false;
}

void UTradeWidget::UpdateGold(int32 Gold)
{
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


