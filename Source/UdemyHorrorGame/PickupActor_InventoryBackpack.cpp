// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupActor_InventoryBackpack.h"
#include "PlayerCharacter.h"
#include "InventoryComponent.h"

void APickupActor_InventoryBackpack::Pickup()
{
	AddMoreSlots(BackpackSlots);
}

void APickupActor_InventoryBackpack::AddMoreSlots(int SlotsAmount)
{
	PlayerCharacter->GetInventoryComponent()->ExpandInventory(SlotsAmount);
	Destroy();
}
