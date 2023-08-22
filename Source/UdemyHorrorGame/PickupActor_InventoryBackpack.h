// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupActor_Main.h"
#include "PickupActor_InventoryBackpack.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYHORRORGAME_API APickupActor_InventoryBackpack : public APickupActor_Main
{
	GENERATED_BODY()

protected:
	bool ExecutePickup() override;


private:
	UPROPERTY(EditAnywhere, Category = Backpack)
	int BackpackSlots = 2;


	void AddMoreSlots(int SlotsAmount);
	
};
