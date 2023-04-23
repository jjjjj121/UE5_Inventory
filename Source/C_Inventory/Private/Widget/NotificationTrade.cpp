// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/NotificationTrade.h"
#include "Components/Button.h"
#include "C_Inventory/Public/Widget/switchingwidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CircularThrobber.h"
#include "C_Inventory/C_InventoryCharacter.h"
#include "Components/TextBlock.h"



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
	TB_MainText->SetText(FText::FromString(TEXT("Would you like to Trade with")));
	TB_Nickname->SetText(FText::FromString(TEXT("")));

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
			///*Client�� ��� ���� ĳ���͸� ������ ĳ���ʹ� ��Ʈ�ѷ��� ������ ���� �ʱ� ������ ��� ��Ʈ�ѷ��� ������ �ִ�
			//�������� TryTrade�� ȣ���ؾ� RPC �Լ��� ȣ���� ������ -> ���� Client������ ������ �ƴ� �������� TryTrade�� ȣ��ǵ��� �ϴ� ������ �ʿ�*/
			OwnCharacter->UserTryTrade();
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

			OwnCharacter->EndTrade(TArray<FItemData>());
			OwnCharacter->UserEndTrade();

		}

		TradeUser = nullptr;
	}
}

void UNotificationTrade::TradeRequest(AC_InventoryCharacter* Character)
{

	if (Character) {
		if (AC_InventoryCharacter* Owner = Cast<AC_InventoryCharacter>(this->GetOwningPlayer()->GetCharacter())) {
			TradeUser = Character;
			
			TB_MainText->SetText(FText::FromString(TEXT("Would you like to Trade with")));
			TB_Nickname->SetText(FText::FromString(TradeUser->Nickname));

			this->SetVisibility(ESlateVisibility::Visible);

		}
	}

}


