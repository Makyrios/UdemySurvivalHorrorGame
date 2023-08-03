// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "HG_PlayerController.h"
#include "PlayerCharacter.h"
#include "InventoryGridWidget.h"
#include "InventorySlotWidget.h"
#include <Kismet/GameplayStatics.h>
#include "Usable.h"
#include "InventoryItem_Usable.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UInventoryComponent::AddItem(TSubclassOf<AInventoryItem_Main> Item, int Amount, int* outRemainder)
{
	AInventoryItem_Main* LocalItem = Item.GetDefaultObject();
	int LocalMaxStackAmount = LocalItem->GetItemData().MaxStackAmount;
	int Remainder = 0;

	// If MaxStackAmount of item > 1, then try to add it to the existing slot
	if (LocalMaxStackAmount > 1)
	{
		int FreeSlotIndex = CheckForFreeSlot(Item);
		if (FreeSlotIndex != -1)
		{
			int SlotAmount = InventorySlots[FreeSlotIndex].Amount;
			int MaxStack = LocalItem->GetItemData().MaxStackAmount;
			int ItemsSum = SlotAmount + Amount;
			InventorySlots[FreeSlotIndex].Amount = FMath::Clamp(ItemsSum, SlotAmount, MaxStack);
			Remainder = ItemsSum - MaxStack;
			if (Remainder > 0)
			{
				AddItem(Item, Remainder, outRemainder);
			}
			UpdateInventorySlot(FreeSlotIndex);
			return true;
		}
	}
	
	// Find free slot and if there exists put in the item
	int EmptySlotIndex = CheckForEmptySlot();
	if (EmptySlotIndex != -1)
	{
		int MaxStack = LocalItem->GetItemData().MaxStackAmount;
		FInventoryItems InvItem(Item, FMath::Clamp(Amount, 0, MaxStack));
		InventorySlots[EmptySlotIndex] = InvItem;
		Remainder = Amount - MaxStack;
		if (Remainder > 0)
		{
			AddItem(Item, Remainder, outRemainder);
		}
		UpdateInventorySlot(EmptySlotIndex);
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("No empty slots: %d"), Amount);
		if (outRemainder != nullptr)
			*outRemainder = Amount;
		return false;
	}
	return false;
}

bool UInventoryComponent::RemoveItem(int Index)
{
	FInventoryItems Item = InventorySlots[Index];
	if (!Item.Item) return false;

	if (Item.Amount > 1)
	{
		InventorySlots[Index].Amount -= 1;
	}
	else
	{
		InventorySlots[Index] = FInventoryItems();
	}
	UpdateInventorySlot(Index);
	return true;
}

TSubclassOf<AInventoryItem_Main> UInventoryComponent::GetItemClassAtIndex(int Index, int* outAmount) const
{
	if (InventorySlots.IsValidIndex(Index) && InventorySlots[Index].Item)
	{
		FInventoryItems item = InventorySlots[Index];
		if (outAmount != nullptr) *outAmount = item.Amount;
		return item.Item;
	}
	return nullptr;
}


void UInventoryComponent::UpdateInventorySlot(int Index)
{
	UInventorySlotWidget* Slot = PlayerCharacter->GetInventoryMenuWidget()->InventoryGrid->GetSlotsArray()[Index];
	if (Slot != nullptr)
	{
		Slot->UpdateSlot();
	}
}

void UInventoryComponent::UseItem(int Index)
{
	TSubclassOf<AInventoryItem_Main> UsableItemClass = GetItemClassAtIndex(Index);
	if (!UsableItemClass)
	{
		UE_LOG(LogTemp, Display, TEXT("Could not find item at index %d"), Index);
		return;
	}

	AActor* ItemActor = UGameplayStatics::GetActorOfClass(this, UsableItemClass);
	if (ItemActor == nullptr)
	{
		ItemActor = GetWorld()->SpawnActor(UsableItemClass);
	}

	if (IUsable* UsableItem = Cast<IUsable>(ItemActor))
	{
		UsableItem->UseItem();
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Could not cast to usable"));
	}
	RemoveItem(Index);
	PlayerCharacter->GetInventoryMenuWidget()->CloseDropdownMenu();
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerContr = Cast<AHG_PlayerController>(GetOwner()->GetInstigatorController());
	PlayerCharacter = Cast<APlayerCharacter>(GetOwner());

	InventorySlots.SetNum(PlayerContr->InventorySlots);
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int UInventoryComponent::CheckForEmptySlot() const
{
	for (int i = 0; i < InventorySlots.Num(); i++)
	{
		if (!InventorySlots[i].Item.GetDefaultObject())
		{
			return i;
		}
	}

	return -1;
}

int UInventoryComponent::CheckForFreeSlot(TSubclassOf<AInventoryItem_Main> ItemClass) const
{
	for (int i = 0; i < InventorySlots.Num(); i++)
	{
		if (InventorySlots[i].Item == ItemClass && InventorySlots[i].Amount < ItemClass.GetDefaultObject()->GetItemData().MaxStackAmount)
		{
			return i;
		}
	}

	return -1;
}

