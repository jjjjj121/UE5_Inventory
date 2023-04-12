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
	/*��ȣ �ۿ�� ĳ���Ϳ��� ������ �Ѱ��ְ� �ı�*/
	/*HasAuthority -> ���������� ����*/
	if (HasAuthority() && Character) {
		Character->AddInventoryItem(ItemData);
		Destroy();
	}

}

void AItem::Use(AC_InventoryCharacter* Character, bool IsInShop, bool IsInTradeWIdget)
{
	if (Character) {
		/*Shop ������ �� ��� ������ ��� ���̱� ������ �κ��丮�� ������ �߰�*/
		if (IsInShop) {
			Character->AddInventoryItem(ItemData);
		}
		/*TradeWidget -> Inventory �� ���*/
		else if (IsInTradeWIdget) {
			Character->AddInventoryItem(ItemData);
		}

	}

}


