// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem_Main.h"
#include "Usable.h"
#include "InventoryItem_Usable.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYHORRORGAME_API AInventoryItem_Usable : public AInventoryItem_Main, public IUsable
{
	GENERATED_BODY()

public:
	bool UseItem() override;
	
protected:
	class APlayerCharacter* PlayerCharacter;

	void BeginPlay() override;
};
