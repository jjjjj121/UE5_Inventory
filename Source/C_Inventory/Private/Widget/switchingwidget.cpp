// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/switchingwidget.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "C_Inventory/Public/Widget/hudwidget.h"
#include "C_Inventory/Public/Widget/Shop.h"

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

int32 Uswitchingwidget::GetActiveWidgetIndex()
{
	return WS_Interface->GetActiveWidgetIndex();
}

void Uswitchingwidget::SwitchingUI(bool IsInventoryUI)
{
	/*inventory*/
	if (IsInventoryUI) {
		WS_Interface->SetActiveWidgetIndex(1);

		if (APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))) {
			PlayerController->SetInputMode(FInputModeGameAndUI());

			/*mouse capture mode �� ��Ŭ���ÿ��� ĸ�ĵǵ��� ����*/
			/*�� ������ ������ ��� GameandUI Mode������ ��Ŭ���� ĸ�ĵǱ� ������ Mouse Button Up �Լ��� ȣ���� �ȵǴ� ���� �߻���*/
			UGameplayStatics::SetViewportMouseCaptureMode(GetWorld(), EMouseCaptureMode::CaptureDuringRightMouseDown);

			PlayerController->SetShowMouseCursor(true);
		}
	}
	/*crosshair*/
	else {
		WS_Interface->SetActiveWidgetIndex(0);

		if (APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))) {
			PlayerController->SetInputMode(FInputModeGameOnly());

			/*Game Mode�� ���ư� �� ���� �⺻ mouse capture mode�� ����*/
			UGameplayStatics::SetViewportMouseCaptureMode(GetWorld(), EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown);
			PlayerController->SetShowMouseCursor(false);
		}
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

void Uswitchingwidget::NativeConstruct()
{
	Super::NativeConstruct();

	W_Shop->ParentWidget = this;
}

