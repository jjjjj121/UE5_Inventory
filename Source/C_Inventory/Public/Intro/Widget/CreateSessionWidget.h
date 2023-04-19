// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreateSessionWidget.generated.h"

class UEditableTextBox;
class UButton;
/**
 * 
 */
UCLASS()
class C_INVENTORY_API UCreateSessionWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:

	UPROPERTY(BlueprintReadOnly, Category ="Create Session Widget", meta = (BindWidget))
		UEditableTextBox* ETB_SessionName;

	UPROPERTY(BlueprintReadOnly, Category = "Create Session Widget", meta = (BindWidget))
		UEditableTextBox* ETB_Password;

	UPROPERTY(BlueprintReadOnly, Category = "Create Session Widget", meta = (BindWidget))
		UButton* BT_Create;

	UPROPERTY(BlueprintReadOnly, Category = "Create Session Widget", meta = (BindWidget))
		UButton* BT_Cancel;


public:

	UEditableTextBox* GetETB_SessionName();
	UEditableTextBox* GetETB_Password();
	UButton* GetBT_Create();
	UButton* GetBT_Cancel();

protected:
	class UC_InventoryGameInstance* GameInstance;

protected:
	virtual void NativeConstruct() override;

public:

	UFUNCTION()
	void OnclickedCreate();

	UFUNCTION()
	void OnclickedCancel();


};
