// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Shop.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "C_Inventory/Public/Widget/InventorySlot.h"
#include "C_Inventory/Public/Actors/Item.h"
#include "C_Inventory/Public/Widget/switchingwidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

UUniformGridPanel* UShop::GetGrid_Shop()
{
	return Grid_Shop;
}

UButton* UShop::GetBT_Close()
{
	return BT_Close;
}

void UShop::NativeConstruct()
{
	Super::NativeConstruct();

	BT_Close->OnClicked.AddDynamic(this, &UShop::CloseButtonClick);

	InitInventory();

}

void UShop::CloseButtonClick()
{
	UE_LOG(LogTemp, Warning, TEXT("CLOSE BUTTON CLICK"));
	ClearShopItem();

	if (ParentWidget) {
		ParentWidget->SwitchingUI(false);
		ParentWidget->W_Shop->SetVisibility(ESlateVisibility::Collapsed);
		if (APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))) {
			PlayerController->SetInputMode(FInputModeGameOnly());
			PlayerController->SetShowMouseCursor(false);
		}
	}

}

void UShop::InitInventory()
{
	for (int i = 0; i < AmountofGrid; ++i) {
		if (SlotWidgetClass) {
			if (UInventorySlot* InventorySlot = Cast<UInventorySlot>(CreateWidget(GetWorld(), SlotWidgetClass))) {
				InventorySlot->IsShopItem = true;
				Grid_Shop->AddChildToUniformGrid(InventorySlot, CurrentRow, CurrentColumn);
				++CurrentColumn;
				if (CurrentColumn >= 4) {
					CurrentColumn = 0;
					++CurrentRow;
				}
			}
		}
	}

	CurrentColumn = 0, CurrentRow = 0;
}

void UShop::AddShopItem(TArray<FItemData> Items, AShopKeeper* NewShopKeeper)
{
	for (FItemData Item : Items) {
		auto AllChildren = Grid_Shop->GetAllChildren();
		for (auto Child : AllChildren) {
			if (UInventorySlot* InventorySlot = Cast<UInventorySlot>(Child)) {
				if (!InventorySlot->ItemData.ItemClass) {
					InventorySlot->ShopKeeper = NewShopKeeper;
					InventorySlot->UpdateItem(Item);
					++TotalInventoryNum;
					break;
				}
			}
		}
	}
}

void UShop::ClearShopItem()
{
	auto AllChildren = Grid_Shop->GetAllChildren();
	for (auto Child : AllChildren) {
		if (UInventorySlot* InventorySlot = Cast<UInventorySlot>(Child)) {
			InventorySlot->DisableButton();
			TotalInventoryNum = 0;
		}
	}

}

void UShop::UpdateShop(TArray<FItemData> Items)
{
	if (Items.Num() < TotalInventoryNum) {
		RemoveShopItem(Items);
	}
	else {
		for (FItemData& Item : Items) {
			auto AllChildren = Grid_Shop->GetAllChildren();
			for (auto Child : AllChildren) {
				if (UInventorySlot* InventorySlot = Cast<UInventorySlot>(Child)) {
					if (InventorySlot->ItemData.ItemClass == Item.ItemClass) {
						InventorySlot->UpdateItem(Item);
						break;
					}
				}
			}
		}
	}
}

void UShop::RemoveShopItem(TArray<FItemData> Items)
{
	auto AllChildren = Grid_Shop->GetAllChildren();

	/*check all inventory slot*/
	for (auto Child : AllChildren) {
		if (UInventorySlot* InventorySlot = Cast<UInventorySlot>(Child)) {
			/*ItemData가 존재하는 Slot*/
			if (InventorySlot->ItemData.ItemClass) {
				if (Items.Num()) {
					for (FItemData& Item : Items) {
						/*해당 슬롯에 등록된 데이터와 새로운 데이터가 다른 경우 -> 소멸되야할 아이템*/
						if (Item.ItemClass != InventorySlot->ItemData.ItemClass) {
							InventorySlot->DisableButton();
						}
					}
				}
				else {
					InventorySlot->DisableButton();
				}
			}
		}
	}


	--TotalInventoryNum;
}



