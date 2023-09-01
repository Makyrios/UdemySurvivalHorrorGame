// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupActor_InventoryBackpack.h"
#include "PlayerCharacter.h"
#include "InventoryComponent.h"

bool APickupActor_InventoryBackpack::ExecutePickup()
{
	AddMoreSlots(BackpackSlots);
	return true;
}

void APickupActor_InventoryBackpack::AddMoreSlots(int SlotsAmount)
{
	PlayerCharacter->GetInventoryComponent()->ExpandInventory(SlotsAmount);
	Destroy();
}
