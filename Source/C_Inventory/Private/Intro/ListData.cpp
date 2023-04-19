// Fill out your copyright notice in the Description page of Project Settings.


#include "Intro/ListData.h"

FString UListData::GetSessionName()
{
	return SessionData.SessionName;
}

FString UListData::GetPassword()
{
	return SessionData.Password;
}

UFindSessionWidget* UListData::GetParentWidget()
{
	return SessionData.ParentWidget;
}

int32 UListData::GetSessionIndex()
{
	return SessionData.SessionIndex;
}

FSessionData UListData::GetSessionData()
{
	return SessionData;
}

void UListData::SetSessionName(FString NewSessionName)
{
	SessionData.SessionName = NewSessionName;
}

void UListData::SetPassword(FString NewPassword)
{
	SessionData.Password = NewPassword;
}

void UListData::SetSessionIndex(int32 NewIndex)
{
	SessionData.SessionIndex = NewIndex;
}

void UListData::SetParentWidget(UFindSessionWidget* NewParentWidget)
{
	SessionData.ParentWidget = NewParentWidget;
}
