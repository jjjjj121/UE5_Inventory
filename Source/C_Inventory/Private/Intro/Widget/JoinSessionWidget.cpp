// Fill out your copyright notice in the Description page of Project Settings.


#include "Intro/Widget/JoinSessionWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Intro/Widget/FindSessionWidget.h"
#include "C_InventoryGameInstance.h"




void UJoinSessionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BT_Join->OnClicked.AddDynamic(this, &UJoinSessionWidget::OnClickedJoin);
	BT_Cancel->OnClicked.AddDynamic(this, &UJoinSessionWidget::OnClickedCancel);
}

void UJoinSessionWidget::OnClickedJoin()
{
	if (SessionData.Password == ETB_Password->Text.ToString()) {
		if (UC_InventoryGameInstance* GameInstance = Cast<UC_InventoryGameInstance>(GetGameInstance())) {
			GameInstance->MyJoinSession(SessionData);
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("PASSWORD IS NOT SAME"));
		TB_Notpassword->SetVisibility(ESlateVisibility::Visible);

		FTimerHandle GravityTimerHandle;
		float GravityTime = 1.5f;
		GetWorld()->GetTimerManager().SetTimer(GravityTimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				// 코드 구현
				TB_Notpassword->SetVisibility(ESlateVisibility::Collapsed);

				// TimerHandle 초기화
				GetWorld()->GetTimerManager().ClearTimer(GravityTimerHandle);

			}), GravityTime, false); //Looping 사용시 true
	}
}

void UJoinSessionWidget::OnClickedCancel()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UJoinSessionWidget::ClickSession(FSessionData NewSessionData)
{
	SessionData = NewSessionData;
	TB_SessionName->SetText(FText::FromString(SessionData.SessionName));
	this->SetVisibility(ESlateVisibility::Visible);
}
