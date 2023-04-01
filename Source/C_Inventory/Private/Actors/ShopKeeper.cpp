// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ShopKeeper.h"
#include "C_Inventory/C_InventoryCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
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

void AShopKeeper::OnRep_Items()
{
	if (AC_InventoryCharacter* Character = Cast<AC_InventoryCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))) {
		if (Character->IsLocallyControlled()) {
		}Character->UpdateShop(Items);
		
	}
}

void AShopKeeper::Interact(AC_InventoryCharacter* Character)
{
	if (Character) {

		Character->OpenShop(Items, this);
	}
}

void AShopKeeper::TransfferedItem(TSubclassOf<AItem> ItemSubclass)
{
	uint8 Index = 0;
	for (FItemData& Item : Items) {
		if (Item.ItemClass == ItemSubclass) {
			--Item.StackCount;
			/*아이템을 모두 소모했을 경우*/
			if (Item.StackCount <= 0) {
				Items.RemoveAt(Index);

			}
			break;
		}
		++Index;
	}
	OnRep_Items();
	//TEST
	//for (FItemData& Item : Items) {
	//	UE_LOG(LogTemp, Warning, TEXT("ITEM STACK : %d"), Item.StackCount);
	//}
}


