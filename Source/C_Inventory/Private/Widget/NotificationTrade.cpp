// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/NotificationTrade.h"
#include "Components/Button.h"
#include "C_Inventory/Public/Widget/switchingwidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CircularThrobber.h"
#include "C_Inventory/C_InventoryCharacter.h"
#include "Components/TextBlock.h"

#define LOCTEXT_NAMESPACE "NotificationTrade"

UButton* UNotificationTrade::GetBT_Yes()
{
	return BT_Yes;
}

UButton* UNotificationTrade::GetBT_No()
{
	return BT_No;
}

UButton* UNotificationTrade::GetBT_Cancel()
{
	return BT_Cancel;
}

UTextBlock* UNotificationTrade::GetTB_MainText()
{
	return TB_MainText;
}

UCircularThrobber* UNotificationTrade::GetCT_LoadingBar()
{
	return CT_LoadingBar;
}

void UNotificationTrade::NativeConstruct()
{
	Super::NativeConstruct();

	/*Bind Event*/
	BT_Yes->OnClicked.AddDynamic(this, &UNotificationTrade::ClickedYes);
	BT_No->OnClicked.AddDynamic(this, &UNotificationTrade::ClickedNo);
	BT_Cancel->OnClicked.AddDynamic(this, &UNotificationTrade::ClickedNo);

}

void UNotificationTrade::InitWidget()
{
	BT_Yes->SetVisibility(ESlateVisibility::Visible);
	BT_No->SetVisibility(ESlateVisibility::Visible);
	BT_Cancel->SetVisibility(ESlateVisibility::Collapsed);
	CT_LoadingBar->SetVisibility(ESlateVisibility::Collapsed);
	TB_MainText->SetText(FText::FromString(TEXT("Would you Trade?")));
	TradeUser = nullptr;
}

void UNotificationTrade::ClickedYes()
{

	/*��û �Ŵ� ���*/
	if (TradeUser) {
		BT_Yes->SetVisibility(ESlateVisibility::Collapsed);
		BT_No->SetVisibility(ESlateVisibility::Collapsed);
		BT_Cancel->SetVisibility(ESlateVisibility::Visible);
		CT_LoadingBar->SetVisibility(ESlateVisibility::Visible);

		TB_MainText->SetText(FText::FromString(TEXT("Waiting for Accept")));

		/*Trade ��û*/
		if (AC_InventoryCharacter* OwnCharacter = Cast<AC_InventoryCharacter>(this->GetOwningPlayer()->GetCharacter())) {

			OwnCharacter->SetWantTrade(true);
			/*Client�� ��� ���� ĳ���͸� ������ ĳ���ʹ� ��Ʈ�ѷ��� ������ ���� �ʱ� ������ ��� ��Ʈ�ѷ��� ������ �ִ�
			�������� TryTrade�� ȣ���ؾ� RPC �Լ��� ȣ���� ������ -> ���� Client������ ������ �ƴ� �������� TryTrade�� ȣ��ǵ��� �ϴ� ������ �ʿ�*/
			if (TradeUser->HasAuthority()) {
				TradeUser->TryTrade(OwnCharacter);
			}
			else {
				OwnCharacter->ClientTryTrade(TradeUser);
			}

		}

		else {
			UE_LOG(LogTemp, Warning, TEXT("Trade Error"));
			TB_MainText->SetText(FText::FromString(TEXT("Trade Error")));
		}
	}

}

void UNotificationTrade::ClickedNo()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
	if (ParentWidget) {
		ParentWidget->OnlyGameMode();
	}

	BT_Yes->SetVisibility(ESlateVisibility::Visible);
	BT_No->SetVisibility(ESlateVisibility::Visible);
	BT_Cancel->SetVisibility(ESlateVisibility::Collapsed);
	CT_LoadingBar->SetVisibility(ESlateVisibility::Collapsed);

	if (TradeUser) {
		if (AC_InventoryCharacter* OwnCharacter = Cast<AC_InventoryCharacter>(this->GetOwningPlayer()->GetCharacter())) {
			OwnCharacter->SetWantTrade(false);
			if (TradeUser->HasAuthority()) {
				TradeUser->EndTrade();
			}
			else {
				OwnCharacter->EndTrade(TradeUser);
			}
		}


		TradeUser = nullptr;
	}
}

void UNotificationTrade::TradeRequest(AC_InventoryCharacter* Character)
{

	if (Character) {
		if (AC_InventoryCharacter* Owner = Cast<AC_InventoryCharacter>(this->GetOwningPlayer()->GetCharacter())) {
			TradeUser = Character;
			TB_MainText->SetText(FText::FromString(TEXT("Would you Trade?")));
			this->SetVisibility(ESlateVisibility::Visible);

		}
	}


}

void UNotificationTrade::SetMainText()
{
	if (TradeUser) {

		/*ID ���� ��� Text Format ����*/
		//FText Text = FText::Format(LOCTEXT("Comment", "Would you Trade for {0} ?"),FText::FromString(TradeUser->UserID));
		//TB_MainText->SetText(Text);
	}
}

#undef LOCTEXT_NAMESPACE

