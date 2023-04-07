// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "C_Inventory/Public/Structs.h"
#include "SlotDragOperation.generated.h"


/**
 * 
 */
UCLASS()
class C_INVENTORY_API USlotDragOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 SlotNum;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FItemData SlotData;




};
