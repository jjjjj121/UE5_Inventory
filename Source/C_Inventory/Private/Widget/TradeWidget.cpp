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
#include "Components/Image.h"


void UTradeWidget::NativeConstruct()
{
    Super::NativeConstruct();
	BT_Close->OnClicked.AddDynamic(this, &UTradeWidget::CloseWidget);
	BT_Gold->OnClicked.AddDynamic(this, &UTradeWidget::OnClickGold);
	ETB_Gold->OnTextCommitted.AddDynamic(this, &UTradeWidget::OnTextCommit);
	BT_Accept->OnClicked.AddDynamic(this, &UTradeWidget::OnClickAccept);
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

	TotalMyInventoryNum = 0, TotalUserInventoryNum = 0;

	UpdateGold(0, true);
	UpdateGold(0, false);

	//TB_User->SetText(FText::FromString(FString::Printf(TEXT("User"))));

	My_AcceptEffect->SetVisibility(ESlateVisibility::Collapsed);
	User_AcceptEffect->SetVisibility(ESlateVisibility::Collapsed);


}

void UTradeWidget::SetUserName()
{
	if (AC_InventoryCharacter* Character = Cast<AC_InventoryCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))) {
		FString Name = Character->TradeCharacter->Nickname;
		TB_User->SetText(FText::FromString(Name));
	}
}

void UTradeWidget::SetAcceptTrade()
{
	User_AcceptEffect->SetVisibility(ESlateVisibility::Visible);

}

TArray<FItemData> UTradeWidget::GetTradeData(bool IsMyTradeData)
{
	auto Children = IsMyTradeData ? Grid_MyTrade->GetAllChildren() : Grid_UserTrade->GetAllChildren();
	TArray<FItemData> TradeItems;
	/*Item -> Inventory*/
	if (AC_InventoryCharacter* Character = Cast<AC_InventoryCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))) {
		for (auto Child : Children) {
			if (UInventorySlot* InventorySlot = Cast<UInventorySlot>(Child)) {
				if (InventorySlot->ItemData.ItemClass) {
					TradeItems.Add(InventorySlot->ItemData);
				}
			}
		}
	}

	/*Gold Data*/
	FItemData GoldData;
	GoldData.ItemClass = AGold::StaticClass();
	GoldData.StackCount = IsMyTradeData ? MyGold : UserGold;
	
	TradeItems.Add(GoldData);

	return TradeItems;
}

void UTradeWidget::CloseWidget()
{
	if (AC_InventoryCharacter* Character = Cast<AC_InventoryCharacter>(this->GetOwningPlayer()->GetCharacter())) {

		Character->EndTrade(GetTradeData(true));
		Character->UserEndTrade();

		/*Reset Widget*/
		Character->TradeReset();
		Character->UserTradeReset();

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

void UTradeWidget::OnClickAccept()
{
	My_AcceptEffect->SetVisibility(ESlateVisibility::Visible);

	/*상대가 이미 수락상태인 경우 -> 거래실행*/
	if (User_AcceptEffect->IsVisible()) {
		/*Item -> Inventory*/
		if (AC_InventoryCharacter* Character = Cast<AC_InventoryCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))) {
			
			/*Update My Items & Gold*/
			Character->SucceedTrade(GetTradeData(false));
			/*Update User Items & Gold*/
			Character->UserSucceedTrade();

			/*Reset Widget*/
			Character->TradeReset();
			Character->UserTradeReset();
		}
	}
	else {
		if (AC_InventoryCharacter* Character = Cast<AC_InventoryCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))) {

			Character->UserSetAcceptTrade();
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

			/*My Trade Widget Update(Gold)*/
			Character->UpdateGold(-TradeValue, true);
			Character->UpdateGold(TradeValue, false);

			/*User Trade Widget Update(Gold)*/
			Character->UserSetUserTradeGold(Character->GetTradeGold() - TradeValue);
			
			ETB_Gold->SetText(FText::FromString(FString::Printf(TEXT(""))));
			ETB_Gold->SetVisibility(ESlateVisibility::Collapsed);

		}
	}
}



