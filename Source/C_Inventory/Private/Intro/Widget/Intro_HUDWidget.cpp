// Fill out your copyright notice in the Description page of Project Settings.


#include "Intro/Widget/Intro_HUDWidget.h"
#include "Components/Button.h"
#include "C_InventoryGameInstance.h"
#include "Intro/Widget/CreateSessionWidget.h"
#include "Intro/Widget/FindSessionWidget.h"


UButton* UIntro_HUDWidget::GetBT_CreateSession()
{
	return BT_CreateSession;
}

UButton* UIntro_HUDWidget::GetBT_JoinSession()
{
	return BT_JoinSession;
}

UCreateSessionWidget* UIntro_HUDWidget::GetW_CreateSession()
{
	return W_CreateSession;
}

UFindSessionWidget* UIntro_HUDWidget::GetW_FindSession()
{
	return W_FindSession;
}

void UIntro_HUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BT_CreateSession->OnClicked.AddDynamic(this, &UIntro_HUDWidget::OnclickedCreateSession);
	BT_JoinSession->OnClicked.AddDynamic(this, &UIntro_HUDWidget::OnclickedJoinSession);

	GameInstance = Cast<UC_InventoryGameInstance>(GetGameInstance());

}

void UIntro_HUDWidget::OnclickedCreateSession()
{
	W_CreateSession->SetVisibility(ESlateVisibility::Visible);
}

void UIntro_HUDWidget::OnclickedJoinSession()
{
	UE_LOG(LogTemp, Warning, TEXT("CLICK JOIN SESSION"));
	W_FindSession->SetVisibility(ESlateVisibility::Visible);

	GameInstance->MyFindSession();

}

void UIntro_HUDWidget::FindSessionComplete(TArray<FOnlineSessionSearchResult> SessionResults)
{
	W_FindSession->FindSessionComplete(SessionResults);
}
