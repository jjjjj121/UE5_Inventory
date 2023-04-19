// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Intro_HUDWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class C_INVENTORY_API UIntro_HUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly, Category ="Intro_Widget", meta = (BindWidget));
	UButton* BT_CreateSession;

	UPROPERTY(BlueprintReadOnly, Category = "Intro_Widget", meta = (BindWidget));
	UButton* BT_JoinSession;

	UPROPERTY(BlueprintReadOnly, Category = "Intro_Widget", meta = (BindWidget));
	class UCreateSessionWidget* W_CreateSession;

	UPROPERTY(BlueprintReadOnly, Category = "Intro_Widget", meta = (BindWidget));
	class UFindSessionWidget* W_FindSession;


public:

	UButton* GetBT_CreateSession();
	UButton* GetBT_JoinSession();
	UCreateSessionWidget* GetW_CreateSession();
	UFindSessionWidget* GetW_FindSession();

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnclickedCreateSession();

	UFUNCTION()
		void OnclickedJoinSession();

protected:
	class UC_InventoryGameInstance* GameInstance;

public:
	void FindSessionComplete(TArray<FOnlineSessionSearchResult> SessionResults);

};
