// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_Inventory/Public/Structs.h"
#include "TradeWidget.generated.h"

class UUniformGridPanel;
class UTextBlock;
class UButton;
class AC_InventoryCharacter;
class Uswitchingwidget;
class UEditableTextBox;
class UImage;
/**
 * 
 */
UCLASS()
class C_INVENTORY_API UTradeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "TradeWidget", meta = (BindWidget))
	UUniformGridPanel* Grid_UserTrade;

	UPROPERTY(BlueprintReadOnly, Category = "TradeWidget", meta = (BindWidget))
	UUniformGridPanel* Grid_MyTrade;

	UPROPERTY(BlueprintReadOnly, Category = "TradeWidget", meta = (BindWidget))
	UTextBlock* TB_UserTradeGold;

	UPROPERTY(BlueprintReadOnly, Category = "TradeWidget", meta = (BindWidget))
	UTextBlock* TB_MyTradeGold;

	UPROPERTY(BlueprintReadOnly, Category = "TradeWidget", meta = (BindWidget))
	UTextBlock* TB_User;

	UPROPERTY(BlueprintReadOnly, Category = "TradeWidget", meta = (BindWidget))
	UButton* BT_Close;

	UPROPERTY(BlueprintReadOnly, Category = "TradeWidget", meta = (BindWidget))
	UButton* BT_Accept;

	UPROPERTY(BlueprintReadOnly, Category = "TradeWidget", meta = (BindWidget))
	UButton* BT_Gold;

	UPROPERTY(BlueprintReadOnly, Category = "TradeWidget", meta = (BindWidget))
	UImage* User_AcceptEffect;

	UPROPERTY(BlueprintReadOnly, Category = "TradeWidget", meta = (BindWidget))
	UImage* My_AcceptEffect;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory", meta = (BindWidget))
	UEditableTextBox* ETB_Gold;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> SlotWidgetClass;

	Uswitchingwidget* ParentWidget;


protected:
	virtual void NativeConstruct() override;

public:
	void InitInventory();
	void AddItem(FItemData ItemData, bool IsMyTradeSlot = true);
	void Update(TArray<FItemData> Items, bool IsMyTradeSlot = true);
	void RemoveItem(TArray<FItemData> Items, bool IsMyTradeSlot = true);
	bool IsNewItem(TArray<FItemData> Items, bool IsMyTradeSlot = true);
	void UpdateGold(int32 Gold, bool IsMyTradeSlot);
	void ResetWidget();
	void SetUserName();
	
	void SetAcceptTrade();
	TArray<FItemData> GetTradeData(bool IsMyTradeData);

protected:
	int32 CurrentRow = 0;
	int32 CurrentColumn = 0;
	int32 TotalMyInventoryNum = 0;
	int32 TotalUserInventoryNum = 0;
	int32 AmountofGrid = 14;

	int32 UserGold = 0;
	int32 MyGold = 0;

public:
	UFUNCTION()
	void CloseWidget();

	UFUNCTION()
	void OnClickGold();

	UFUNCTION()
	void OnClickAccept();

	UFUNCTION()
	void OnTextCommit(const FText& Text, ETextCommit::Type CommitMethod);



};
