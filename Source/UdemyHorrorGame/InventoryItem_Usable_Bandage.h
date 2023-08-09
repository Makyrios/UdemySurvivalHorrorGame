// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem_Usable.h"
#include "InventoryItem_Usable_Bandage.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYHORRORGAME_API AInventoryItem_Usable_Bandage : public AInventoryItem_Usable
{
	GENERATED_BODY()

public:
	bool UseItem() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Item)
	float HealAmount = 20;
	
};
