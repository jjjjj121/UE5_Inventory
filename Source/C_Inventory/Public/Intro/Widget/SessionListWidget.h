// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_Inventory/Public/Structs.h"
#include "SessionListWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class C_INVENTORY_API USessionListWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "Find Session Widget", meta = (BindWidget))
		class UButton* BT_Session;

	UPROPERTY(BlueprintReadOnly, Category = "Find Session Widget", meta = (BindWidget))
		UTextBlock* TB_SessionName;

	UPROPERTY(BlueprintReadOnly, Category = "Find Session Widget", meta = (BindWidget))
		UTextBlock* TB_Nickname;

private:
	FSessionData SessionData;

private:
	void SetSessionName(FString NewSessionName);
	void SetNickName(FString NewNickName);

protected:
	virtual void NativeConstruct() override;


protected:
	UFUNCTION()
	void OnclickedSession();

public:
	UFUNCTION(BlueprintCallable)
	void SetData(class UListData* NewData);




};
