// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/hudwidget.h"
#include "Components/TextBlock.h"

UTextBlock* Uhudwidget::GetTB_Hunger()
{
	return TB_Hunger;
}

UTextBlock* Uhudwidget::GetTB_Health()
{
	return TB_Health;
}

void Uhudwidget::UpdateStats(float Hunger, float Health)
{
	TB_Hunger->SetText(FText::FromString(FString::Printf(TEXT("%d"), static_cast<int32>(Hunger))));
	TB_Health->SetText(FText::FromString(FString::Printf(TEXT("%d"), static_cast<int32>(Health))));

}
