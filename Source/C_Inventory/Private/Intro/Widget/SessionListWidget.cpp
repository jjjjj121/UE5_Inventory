// Fill out your copyright notice in the Description page of Project Settings.


#include "Intro/Widget/SessionListWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Intro/ListData.h"
#include "Intro/Widget/FindSessionWidget.h"



void USessionListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BT_Session->OnClicked.AddDynamic(this, &USessionListWidget::OnclickedSession);

}

void USessionListWidget::SetData(UListData* NewData)
{
	SessionData = NewData->GetSessionData();

	SetSessionName(SessionData.SessionName);
	SetNickName(SessionData.Nickname);
}

void USessionListWidget::SetSessionName(FString NewSessionName)
{
	TB_SessionName->SetText(FText::FromString(NewSessionName));
}

void USessionListWidget::SetNickName(FString NewNickName)
{
	TB_Nickname->SetText(FText::FromString(NewNickName));
}

void USessionListWidget::OnclickedSession()
{
	if (SessionData.ParentWidget) {
		SessionData.ParentWidget->ClickSession(SessionData);
	}
}
