// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryItems.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UDEMYHORRORGAME_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	bool AddItem(TSubclassOf<AInventoryItem_Main> Item, int Amount, int* outRemainder = nullptr);

	FItemData GetItemDataAtIndex(int Index, int* Amount = nullptr) const;

	void UpdateInventorySlot(int Index);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	TArray<FInventoryItems> InventorySlots;

	
	class AHG_PlayerController* PlayerContr;
	class APlayerCharacter* PlayerCharacter;

	int CheckForEmptySlot() const;
	int CheckForFreeSlot(TSubclassOf<AInventoryItem_Main> ItemClass) const;
};
