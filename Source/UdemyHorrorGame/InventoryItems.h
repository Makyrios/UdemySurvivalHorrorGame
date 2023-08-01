// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem_Main.h"
#include "InventoryItems.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItems
{
	GENERATED_BODY()
	
	FInventoryItems()
	{
		Item = nullptr;
		Amount = 0;
	}

	FInventoryItems(TSubclassOf<AInventoryItem_Main> Item_, int Amount_)
	{
		Item = Item_;
		Amount = Amount_;
	}

	UPROPERTY(EditAnywhere, Category = Items)
	TSubclassOf<AInventoryItem_Main> Item;

	UPROPERTY(EditAnywhere, Category = Items)
	int Amount;
};
