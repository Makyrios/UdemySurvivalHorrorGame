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

	/// <summary>
	/// Add item to the array and update widget
	/// </summary>
	/// <param name="Item">Item class to add</param>
	/// <param name="Amount">Amount of item</param>
	/// <param name="outRemainder">How many items are left</param>
	/// <returns>Success of adding item</returns>
	bool AddItem(TSubclassOf<AInventoryItem_Main> Item, int Amount, int* outRemainder = nullptr);

	bool RemoveItem(int Index);

	/// <summary>
	/// Get class of item in the array by index
	/// </summary>
	/// <param name="Index">Index of the item in the array</param>
	/// <param name="Amount">Amount of item</param>
	/// <returns>If index is valid return FItemData of the item, otherwise FItemData()</returns>
	TSubclassOf<AInventoryItem_Main> GetItemClassAtIndex(int Index, int* outAmount = nullptr) const;

	/// <summary>
	/// Update inventory slot by index
	/// </summary>
	/// <param name="Index">Index of the slot</param>
	void UpdateInventorySlot(int Index);

	void UseItem(int Index);

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
