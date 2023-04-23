// Fill out your copyright notice in the Description page of Project Settings.


#include "Intro/Widget/FindSessionWidget.h"
#include "Components/CircularThrobber.h"
#include "Components/ListView.h"
#include "OnlineSessionSettings.h"
#include "Intro/ListData.h"
#include "Intro/Widget/JoinSessionWidget.h"


void UFindSessionWidget::FindSessionComplete(TArray<FOnlineSessionSearchResult> SessionResults)
{
	int32 Index = 0;

	
	for (FOnlineSessionSearchResult Reuslt : SessionResults) {

		/*Create Object*/
		UListData* SessionData = NewObject<UListData>(this, UListData::StaticClass());

		FOnlineSessionSettings SessionSettings = Reuslt.Session.SessionSettings;

		/*Custom Setting 을 Object에 전달*/
		for (auto CustomSetting : SessionSettings.Settings) {
			FName Key = CustomSetting.Key;
			FString Value = *CustomSetting.Value.Data.ToString();

			UE_LOG(LogTemp, Warning, TEXT("Custom Key : %s"), *Key.ToString());
			UE_LOG(LogTemp, Warning, TEXT("Custom Value : %s"), *Value);

			if (Key == FName(TEXT("sessionname"))) {
				SessionData->SetSessionName(Value);
			}
			else if (Key == FName(TEXT("password"))) {
				SessionData->SetPassword(Value);
			}
			else if (Key == FName(TEXT("nickname"))) {
				SessionData->SetNickname(Value);
			}

		}

		SessionData->SetParentWidget(this);
		SessionData->SetSessionIndex(Index);
		
		LV_SessionList->AddItem(SessionData);

		++Index;
	}

}


void UFindSessionWidget::ClickSession(FSessionData NewSessionData)
{
	W_JoinSession->ClickSession(NewSessionData);
}



