// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_Inventory/Public/Structs.h"
#include "FindSessionWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class C_INVENTORY_API UFindSessionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "Find Session Widget", meta = (BindWidget))
	class UCircularThrobber* CT_Loading;

	UPROPERTY(BlueprintReadOnly, Category = "Find Session Widget", meta = (BindWidget))
	class UListView* LV_SessionList;

	UPROPERTY(BlueprintReadOnly, Category = "Find Session Widget", meta = (BindWidget))
	class UJoinSessionWidget* W_JoinSession;

public:
	void FindSessionComplete(TArray<FOnlineSessionSearchResult> SessionResults);
	void ClickSession(FSessionData NewSessionData);


};
