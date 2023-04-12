// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/TradeWidget.h"
#include "C_Inventory/Public/Widget/InventorySlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "C_Inventory/C_InventoryCharacter.h"
#include "C_Inventory/Public/Widget/switchingwidget.h"
#include "C_Inventory/Public/Actors/Item.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "C_Inventory/Public/Actors/Gold.h"


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

UButton* UTradeWidget::GetBT_Gold()
{
	return BT_Gold;
}

UEditableTextBox* UTradeWidget::GetETB_Gold()
{
	return ETB_Gold;
}

void UTradeWidget::NativeConstruct()
{
    Super::NativeConstruct();
	BT_Close->OnClicked.AddDynamic(this, &UTradeWidget::CloseWidget);
	BT_Gold->OnClicked.AddDynamic(this, &UTradeWidget::OnClickGold);
	ETB_Gold->OnTextCommitted.AddDynamic(this, &UTradeWidget::OnTextCommit);
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
				UserTradeSlot->IsMyTradeItem = false; MyTradeSlot->IsMyTradeItem = true;
				
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

void UTradeWidget::AddItem(FItemData ItemData, bool IsMyTradeSlot)
{

	auto AllChildren = IsMyTradeSlot ? Grid_MyTrade->GetAllChildren() : Grid_UserTrade->GetAllChildren();
	for (auto Child : AllChildren) {
		if (UInventorySlot* InventorySlot = Cast<UInventorySlot>(Child)) {
			if (!InventorySlot->ItemData.ItemClass) {
				InventorySlot->UpdateItem(ItemData);
				IsMyTradeSlot ? ++TotalMyInventoryNum : ++TotalUserInventoryNum;
				//UE_LOG(LogTemp, Warning, TEXT("Trade Widget Add Item : %d"), IsMyTradeSlot ? TotalMyInventoryNum : TotalUserInventoryNum);
				break;
			}
		}
	}
}

void UTradeWidget::Update(TArray<FItemData> Items, bool IsMyTradeSlot)
{
	
	if (Items.Num() < (IsMyTradeSlot ? TotalMyInventoryNum : TotalUserInventoryNum)) {
		RemoveItem(Items, IsMyTradeSlot);
	}
	else {
		for (FItemData& Item : Items) {
			auto AllChildren = IsMyTradeSlot ? Grid_MyTrade->GetAllChildren() : Grid_UserTrade->GetAllChildren();
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

void UTradeWidget::RemoveItem(TArray<FItemData> Items, bool IsMyTradeSlot)
{
	auto AllChildren = IsMyTradeSlot ? Grid_MyTrade->GetAllChildren() : Grid_UserTrade->GetAllChildren();

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


	IsMyTradeSlot ? --TotalMyInventoryNum : --TotalUserInventoryNum;
}

bool UTradeWidget::IsNewItem(TArray<FItemData> Items, bool IsMyTradeSlot)
{
	return IsMyTradeSlot ? Items.Num() > TotalMyInventoryNum : Items.Num() > TotalUserInventoryNum;
}

void UTradeWidget::UpdateGold(int32 Gold, bool IsMyTradeSlot)
{
	//UE_LOG(LogTemp, Warning, TEXT("UPDATE TRADE GOLD"));
	if (IsMyTradeSlot) {
		MyGold = Gold;
		TB_MyTradeGold->SetText(FText::FromString(FString::Printf(TEXT("%d"), Gold)));
	}
	else {
		UserGold = Gold;
		TB_UserTradeGold->SetText(FText::FromString(FString::Printf(TEXT("%d"), Gold)));
	}
	
}

void UTradeWidget::ResetWidget()
{
	auto Children = Grid_MyTrade->GetAllChildren();
	for (auto Child : Children) {
		if (UInventorySlot* InventorySlot = Cast<UInventorySlot>(Child)) {
			InventorySlot->UpdateItem(FItemData());
		}
	}

	Children = Grid_UserTrade->GetAllChildren();
	for (auto Child : Children) {
		if (UInventorySlot* InventorySlot = Cast<UInventorySlot>(Child)) {
			InventorySlot->UpdateItem(FItemData());
		}
	}

	UpdateGold(0, true);
	UpdateGold(0, false);
}

void UTradeWidget::CloseWidget()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
	if (ParentWidget) {
		ParentWidget->SwitchingUI(false);
	}
	if (AC_InventoryCharacter* OwnCharacter = Cast<AC_InventoryCharacter>(this->GetOwningPlayer()->GetCharacter())) {
		OwnCharacter->SetWantTrade(false);
		if (ParentWidget->TradeCharacter->HasAuthority()) {
			ParentWidget->TradeCharacter->EndTrade();
		}
		else {
			OwnCharacter->EndTrade(ParentWidget->TradeCharacter);
		}
	}
	

}

void UTradeWidget::OnClickGold()
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

void UTradeWidget::OnTextCommit(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter) {
		//UE_LOG(LogTemp, Warning, TEXT("ON TEXT COMMIT : %s"), *Text.ToString());
		if (AC_InventoryCharacter* Character = Cast<AC_InventoryCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))) {
			int32 TradeValue = FCString::Atof(*Text.ToString());
			Character->GetTradeGold() > TradeValue ? TradeValue : TradeValue = Character->GetTradeGold();

			FItemData GoldData;
			GoldData.ItemClass = AGold::StaticClass();
			GoldData.StackCount = TradeValue;

			Character->UpdateGold(-TradeValue, true);
			Character->UpdateGold(TradeValue, false);

			if (ParentWidget->TradeCharacter->HasAuthority()) {
				ParentWidget->TradeCharacter->SetUserTradeGold(Character->GetTradeGold());
			}
			else {
				Character->ClientSetUserTradeGold(Character->GetTradeGold() - TradeValue);
			}
			
			
			ETB_Gold->SetText(FText::FromString(FString::Printf(TEXT(""))));
			ETB_Gold->SetVisibility(ESlateVisibility::Collapsed);

		}
	}
}



