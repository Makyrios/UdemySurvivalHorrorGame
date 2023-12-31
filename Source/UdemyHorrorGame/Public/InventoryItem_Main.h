// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemData.h"
#include "InventoryItem_Main.generated.h"

UCLASS()
class UDEMYHORRORGAME_API AInventoryItem_Main : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInventoryItem_Main();

	inline const FItemData GetItemData() const { return ItemData; }

protected:
	UPROPERTY(EditAnywhere, Category = Item)
	FItemData ItemData;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
