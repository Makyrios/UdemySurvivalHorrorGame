// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryItems.h"
#include "ExaminationWidget.h"
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

	/// <summary>
	/// Remove item at index and update widget
	/// </summary>
	/// <param name="Index">Index of the slot</param>
	/// <returns>Success</returns>
	bool RemoveItem(int Index);

	/// <summary>
	/// Get class of item in the array by index
	/// </summary>
	/// <param name="Index">Index of the item in the array</param>
	/// <param name="outAmount">Out amount of item</param>
	/// <returns>If index is valid return FItemData of the item, otherwise FItemData()</returns>
	TSubclassOf<AInventoryItem_Main> GetItemClassAtIndex(int Index, int* outAmount = nullptr) const;

	/// <summary>
	/// Update inventory slot by index
	/// </summary>
	/// <param name="Index">Index of the slot</param>
	void UpdateInventorySlot(int Index);

	/// <summary>
	/// Use item at index and remove it
	/// </summary>
	/// <param name="Index">Index of the slot</param>
	void UseItem(int Index);

	/// <summary>
	/// Open examining widget
	/// </summary>
	/// <param name="Index">Index of the slot</param>
	void ExamineItem(int Index);

	/// <summary>
	/// Drop item from inventory
	/// </summary>
	/// <param name="Index">Index of the slot</param>
	void DropItem(int Index);

	void ExpandInventory(int Amount);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Initialize();


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY(EditAnywhere, Category = Item)
	float DropLength = 300;

	UPROPERTY(EditDefaultsOnly, Category = HUD)
	TSubclassOf<class UInventoryMenuWidget> InventoryMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = HUD)
	TSubclassOf<UExaminationWidget> ExaminationWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = HUD)
	TSubclassOf<AActor> ExaminationActorClass;

public:
	inline class AExamination* GetExaminationActor() const { return ExaminationActor; }
	inline class UInventoryMenuWidget* GetInventoryMenuWidget() const { return InventoryMenuWidget; }
	inline const TArray<FInventoryItems>& GetInventorySlots() const { return InventorySlots; }

private:
	TArray<FInventoryItems> InventorySlots;
	
	class AHG_PlayerController* PlayerContr;
	class APlayerCharacter* PlayerCharacter;
	class UInventoryMenuWidget* InventoryMenuWidget;
	class UExaminationWidget* ExaminationWidget;
	class AExamination* ExaminationActor;

	int CheckForEmptySlot() const;
	int CheckForFreeSlot(TSubclassOf<AInventoryItem_Main> ItemClass) const;

};
