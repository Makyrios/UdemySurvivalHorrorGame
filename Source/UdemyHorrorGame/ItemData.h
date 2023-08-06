// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.generated.h"

class APickupActor_Main;

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, Category = "Item")
	FName ItemName;
	UPROPERTY(EditAnywhere, Category = "Item")
	UTexture2D* Icon;
	UPROPERTY(EditAnywhere, Category = "Item")
	FName Description;
	UPROPERTY(EditAnywhere, Category = "Item")
	int MaxStackAmount;
	UPROPERTY(EditAnywhere, Category = "Item")
	bool bCanBeUsed = true;
	UPROPERTY(EditAnywhere, Category = "Item")
	bool bCanBeDropped = true;
	UPROPERTY(EditAnywhere, Category = "Item")
	TSubclassOf<APickupActor_Main> PickupActor;
};
