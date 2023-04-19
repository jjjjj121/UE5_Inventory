// Fill out your copyright notice in the Description page of Project Settings.


#include "Intro/Widget/SessionListWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Intro/ListData.h"
#include "Intro/Widget/FindSessionWidget.h"

UButton* USessionListWidget::GetBT_Session()
{
	return BT_Session;
}

UTextBlock* USessionListWidget::GetTB_SessionName()
{
	return TB_SessionName;
}

void USessionListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BT_Session->OnClicked.AddDynamic(this, &USessionListWidget::OnclickedSession);

}

void USessionListWidget::SetData(UListData* NewData)
{
	SessionData = NewData->GetSessionData();

	SetSessionName(SessionData.SessionName);
	
}

void USessionListWidget::SetSessionName(FString NewSessionName)
{
	TB_SessionName->SetText(FText::FromString(NewSessionName));
}

void USessionListWidget::OnclickedSession()
{
	if (SessionData.ParentWidget) {
		SessionData.ParentWidget->ClickSession(SessionData);
	}
}
