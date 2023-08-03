// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem_Usable.h"
#include "InventoryItem_FlashlightBattery.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYHORRORGAME_API AInventoryItem_FlashlightBattery : public AInventoryItem_Usable
{
	GENERATED_BODY()

public:
	void UseItem() override;
	
};
