// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "C_Inventory/Public/Structs.h"
#include "ListData.generated.h"

class UFindSessionWidget;
/**
 * 
 */
UCLASS(Blueprintable)
class C_INVENTORY_API UListData : public UObject
{
	GENERATED_BODY()
	
private:

	FSessionData SessionData;


public:
	FString GetSessionName();
	FString GetPassword();
	UFindSessionWidget* GetParentWidget();
	int32 GetSessionIndex();
	FSessionData GetSessionData();

public:
	void SetSessionName(FString NewSessionName);
	void SetPassword(FString NewPassword);
	void SetSessionIndex(int32 NewIndex);
	void SetParentWidget(UFindSessionWidget* NewParentWidget);

};
