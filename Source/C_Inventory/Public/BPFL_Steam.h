// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BPFL_Steam.generated.h"

/**
 * 
 */
UCLASS()
class C_INVENTORY_API UBPFL_Steam : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		static FString GetSteamIDByString();

};
