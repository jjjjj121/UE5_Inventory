// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NotificationTrade.generated.h"

class UButton;
class UCircularThrobber;
class Uswitchingwidget;
class AC_InventoryCharacter;
class UTextBlock;
/**
 * 
 */
UCLASS()
class C_INVENTORY_API UNotificationTrade : public UUserWidget
{
	GENERATED_BODY()
	

public:

	UPROPERTY(BlueprintReadOnly, Category = "Notify Trade", meta = (BindWidget))
	UButton* BT_Yes;
	
	UPROPERTY(BlueprintReadOnly, Category = "Notify Trade", meta = (BindWidget))
	UButton* BT_No;

	UPROPERTY(BlueprintReadOnly, Category = "Notify Trade", meta = (BindWidget))
	UButton* BT_Cancel;
	
	UPROPERTY(BlueprintReadOnly, Category = "Notify Trade", meta = (BindWidget))
	UTextBlock* TB_MainText;

	UPROPERTY(BlueprintReadOnly, Category = "Notify Trade", meta = (BindWidget))
	UCircularThrobber* CT_LoadingBar;
	
	Uswitchingwidget* ParentWidget;

protected:
	AC_InventoryCharacter* TradeUser;
	

public:
	UButton* GetBT_Yes();
	UButton* GetBT_No();
	UButton* GetBT_Cancel();
	UTextBlock* GetTB_MainText();
	UCircularThrobber* GetCT_LoadingBar();

public:
	
	virtual void NativeConstruct() override;
	void InitWidget();
	
public:
	UFUNCTION()
	void ClickedYes();

	UFUNCTION()
	void ClickedNo();

	void TradeRequest(AC_InventoryCharacter* Character);

	/*ID만들경우*/
	void SetMainText();


};
