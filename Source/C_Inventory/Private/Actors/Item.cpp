// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Item.h"
#include "C_Inventory/C_InventoryCharacter.h"


// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = ItemMesh;


	bReplicates = true;

	ItemData.ItemClass = StaticClass();

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItem::Interact(class AC_InventoryCharacter* Character)
{
	/*상호 작용시 캐릭터에게 데이터 넘겨주고 파괴*/
	/*HasAuthority -> 서버에서만 실행*/
	if (HasAuthority() && Character){
		Character->AddInventoryItem(ItemData);
		Destroy();
	}
	
}

void AItem::Use(AC_InventoryCharacter* Character)
{

	UE_LOG(LogTemp, Warning, TEXT("Using Item : %s"), *GetName());

}


