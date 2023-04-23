// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/hudwidget.h"
#include "Components/TextBlock.h"
#include "C_Inventory/C_InventoryCharacter.h"
#include "Kismet/Gameplaystatics.h"

void Uhudwidget::UpdateStats(float Hunger, float Health)
{
	TB_Hunger->SetText(FText::FromString(FString::Printf(TEXT("%d"), static_cast<int32>(Hunger))));
	TB_Health->SetText(FText::FromString(FString::Printf(TEXT("%d"), static_cast<int32>(Health))));

}

void Uhudwidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (AC_InventoryCharacter* Character = Cast<AC_InventoryCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))) {
		if (Character->HasAuthority()) {
			TB_ClientServer->SetText(FText::FromString(FString::Printf(TEXT("Server"))));
		}
		else {
			TB_ClientServer->SetText(FText::FromString(FString::Printf(TEXT("Client"))));
		}
	}
}
