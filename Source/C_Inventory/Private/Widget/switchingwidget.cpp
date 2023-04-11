// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/switchingwidget.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "C_Inventory/Public/Widget/hudwidget.h"
#include "C_Inventory/Public/Widget/Shop.h"
#include "C_Inventory/C_InventoryCharacter.h"
#include "C_Inventory/Public/Widget/TradeWidget.h"
#include "C_Inventory/Public/Widget/NotificationTrade.h"
#include "C_Inventory/Public/Widget/Inventory.h"


UInventory* Uswitchingwidget::GetW_Inventory()
{
	return W_Inventory;
}

Uhudwidget* Uswitchingwidget::GetW_HUD()
{
	return W_HUD;
}

UWidgetSwitcher* Uswitchingwidget::GetWS_Interface()
{
	
	return WS_Interface;
}

UShop* Uswitchingwidget::GetW_Shop()
{
	return W_Shop;
}

UTradeWidget* Uswitchingwidget::GetW_TradeWidget()
{
	return W_TradeWidget;
}

UNotificationTrade* Uswitchingwidget::GetW_NotificationTrade()
{
	return W_NotificationTrade;
}

int32 Uswitchingwidget::GetActiveWidgetIndex()
{
	return WS_Interface->GetActiveWidgetIndex();
}

void Uswitchingwidget::SwitchingUI(bool IsInventoryUI)
{
	/*inventory*/
	if (IsInventoryUI) {
		WS_Interface->SetActiveWidgetIndex(1);

		GameandUIMode();
	}
	/*crosshair*/
	else {
		WS_Interface->SetActiveWidgetIndex(0);

		OnlyGameMode();
	}

}

void Uswitchingwidget::UpdateStats(float Hunger, float Health)
{
	W_HUD->UpdateStats(Hunger, Health);
}

void Uswitchingwidget::OpenShopUI()
{
	SwitchingUI(true);
	W_Shop->SetVisibility(ESlateVisibility::Visible);

}

void Uswitchingwidget::TradeRequest(AC_InventoryCharacter* Character)
{
	//UE_LOG(LogTemp, Warning, TEXT("Server : %d"), Character->WantTrade);

	if (W_NotificationTrade) {
		W_NotificationTrade->TradeRequest(Character);
		
		GameandUIMode();
	}


}

void Uswitchingwidget::OnTrade(AC_InventoryCharacter* TradeUser)
{

	W_NotificationTrade->SetVisibility(ESlateVisibility::Collapsed);
	W_NotificationTrade->InitWidget();

	W_TradeWidget->TradeCharacter = TradeUser;
	W_TradeWidget->SetVisibility(ESlateVisibility::Visible);

	SwitchingUI(true);

}

void Uswitchingwidget::EndTrade()
{
	
	W_TradeWidget->TradeCharacter = nullptr;
	W_TradeWidget->SetVisibility(ESlateVisibility::Collapsed);

	W_NotificationTrade->SetVisibility(ESlateVisibility::Collapsed);
	W_NotificationTrade->InitWidget();

	SwitchingUI(false);

}

void Uswitchingwidget::UpdateTradeGold(int32 TradeGold)
{
	W_TradeWidget->UpdateGold(TradeGold);
}


void Uswitchingwidget::NativeConstruct()
{
	Super::NativeConstruct();

	W_Inventory->ParentWidget = this;
	W_Shop->ParentWidget = this;
	W_NotificationTrade->ParentWidget = this;
	W_TradeWidget->ParentWidget = this;
}

void Uswitchingwidget::GameandUIMode()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))) {
		PlayerController->SetInputMode(FInputModeGameAndUI());

		/*mouse capture mode 를 우클릭시에만 캡쳐되도록 변경*/
		/*이 설정을 안해줄 경우 GameandUI Mode에서는 좌클릭시 캡쳐되기 때문에 Mouse Button Up 함수가 호출이 안되는 현상 발생함*/
		UGameplayStatics::SetViewportMouseCaptureMode(GetWorld(), EMouseCaptureMode::CaptureDuringRightMouseDown);

		PlayerController->SetShowMouseCursor(true);
	}
}

void Uswitchingwidget::OnlyGameMode()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))) {
		PlayerController->SetInputMode(FInputModeGameOnly());

		/*Game Mode로 돌아갈 때 원래 기본 mouse capture mode로 변경*/
		UGameplayStatics::SetViewportMouseCaptureMode(GetWorld(), EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown);
		PlayerController->SetShowMouseCursor(false);
	}
}

