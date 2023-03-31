// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ShopKeeper.h"
#include "C_Inventory/C_InventoryCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AShopKeeper::AShopKeeper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ShopKeeperMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	RootComponent = ShopKeeperMesh;

	bReplicates = true;
	

}

void AShopKeeper::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShopKeeper, Items);
}

// Called when the game starts or when spawned
void AShopKeeper::BeginPlay()
{
	Super::BeginPlay();
	
}

void AShopKeeper::Interact(AC_InventoryCharacter* Character)
{
	if (Character) {
		Character->OpenShop(Items);
	}
}


