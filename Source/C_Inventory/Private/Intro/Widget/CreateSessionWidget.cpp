// Fill out your copyright notice in the Description page of Project Settings.


#include "Intro/Widget/CreateSessionWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "C_InventoryGameInstance.h"

UEditableTextBox* UCreateSessionWidget::GetETB_SessionName()
{
	return ETB_SessionName;
}

UEditableTextBox* UCreateSessionWidget::GetETB_Password()
{
	return ETB_Password;
}

UButton* UCreateSessionWidget::GetBT_Create()
{
	return BT_Create;
}

UButton* UCreateSessionWidget::GetBT_Cancel()
{
	return BT_Cancel;
}

void UCreateSessionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BT_Create->OnClicked.AddDynamic(this, &UCreateSessionWidget::OnclickedCreate);
	BT_Cancel->OnClicked.AddDynamic(this, &UCreateSessionWidget::OnclickedCancel);

	GameInstance = Cast<UC_InventoryGameInstance>(GetGameInstance());
}

void UCreateSessionWidget::OnclickedCreate()
{
	FName NewSessionName = FName(*ETB_SessionName->Text.ToString());
	FName NewPassword = FName(*ETB_Password->Text.ToString());
	if (!NewSessionName.IsNone()) {
		GameInstance->MyCreateSession(NewSessionName, NewPassword);
	}
	
}

void UCreateSessionWidget::OnclickedCancel()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}
