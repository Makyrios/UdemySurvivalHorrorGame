

// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem_Usable.h"
#include "PlayerCharacter.h"
#include <Kismet/GameplayStatics.h>

bool AInventoryItem_Usable::UseItem()
{
	return true;
}

void AInventoryItem_Usable::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
}
