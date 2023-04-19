#pragma once

#include "Structs.generated.h"

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class AItem> ItemClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UTexture2D* ItemImage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float ItemCost = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StackCount = 1;

public:
	void SetDefault() { ItemClass = 0, ItemImage = 0, ItemCost = 0, StackCount = 1; };
};

USTRUCT(BlueprintType)
struct FSessionData
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SessionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Password;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UFindSessionWidget* ParentWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SessionIndex = 0;


};
