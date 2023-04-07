// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DragItemVisual.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class C_INVENTORY_API UDragItemVisual : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadOnly, Category = "DragItemVisual", meta = (BindWidget))
	UImage* Img_DragItem;
	
public:
	UImage* GetImg_DragItem();




};
