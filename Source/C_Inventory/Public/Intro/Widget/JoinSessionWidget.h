// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_Inventory/Public/Structs.h"
#include "JoinSessionWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class C_INVENTORY_API UJoinSessionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Find Session Widget", meta = (BindWidget))
	class UTextBlock* TB_SessionName;

	UPROPERTY(BlueprintReadOnly, Category = "Find Session Widget", meta = (BindWidget))
	class UEditableTextBox* ETB_Password;

	UPROPERTY(BlueprintReadOnly, Category = "Find Session Widget", meta = (BindWidget))
	UButton* BT_Join;

	UPROPERTY(BlueprintReadOnly, Category = "Find Session Widget", meta = (BindWidget))
	UButton* BT_Cancel;

private:

	FSessionData SessionData;
	//FString Password;
	//UListData SessionData;

protected:
	virtual void NativeConstruct() override;


protected:
	UFUNCTION()
	void OnClickedJoin();
	UFUNCTION()
	void OnClickedCancel();

public:
	void ClickSession(FSessionData NewSessionData);

};
