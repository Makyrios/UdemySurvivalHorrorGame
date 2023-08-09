// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "HG_PlayerController.h"
#include "PlayerCharacter.h"
#include "InventoryGridWidget.h"
#include "InventorySlotWidget.h"
#include <Kismet/GameplayStatics.h>
#include "Usable.h"
#include "InventoryItem_Usable.h"
#include "Camera/CameraComponent.h"
#include "PickupActor_Main.h"
#include "Examination.h"
#include "InventoryMenuWidget.h"

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
	UInventorySlotWidget* Slot = InventoryMenuWidget->InventoryGrid->GetSlotsArray()[Index];
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
		UE_LOG(LogTemp, Warning, TEXT("Could not find item at index %d"), Index);
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
	// Close menu after removing the item
	PlayerCharacter->GetInventoryComponent()->GetInventoryMenuWidget()->CloseDropdownMenu();
}

void UInventoryComponent::ExamineItem(int Index)
{
	ExaminationWidget->UpdateWidget(Index);
	InventoryMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	ExaminationWidget->AddToViewport(2);
	PlayerCharacter->GetInventoryComponent()->GetInventoryMenuWidget()->CloseDropdownMenu();
	PlayerCharacter->bIsInventoryOpen = false;
}

void UInventoryComponent::DropItem(int Index)
{
	TSubclassOf<AInventoryItem_Main> ItemClass = GetItemClassAtIndex(Index);
	if (!ItemClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not find item at index %d"), Index);
		return;
	}
	
	UCameraComponent* CameraComponent = PlayerCharacter->GetCameraComponent();
	FHitResult HitResult;
	FVector Start = CameraComponent->GetComponentLocation();
	FVector End = Start + CameraComponent->GetForwardVector() * DropLength;
	bool bWasHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility);
	APickupActor_Main* SpawnedActor = nullptr;
	if (bWasHit)
	{
		SpawnedActor = Cast<APickupActor_Main>(GetWorld()->SpawnActor(ItemClass.GetDefaultObject()->GetItemData().PickupActor, &HitResult.Location));
	}
	else
	{
		SpawnedActor = Cast<APickupActor_Main>(GetWorld()->SpawnActor(ItemClass.GetDefaultObject()->GetItemData().PickupActor, &End));
	}
	SpawnedActor->StaticMesh->SetSimulatePhysics(true);
	RemoveItem(Index);
	PlayerCharacter->GetInventoryComponent()->GetInventoryMenuWidget()->CloseDropdownMenu();
}

void UInventoryComponent::ExpandInventory(int Amount)
{
	if (Amount > 0)
	{
		InventorySlots.SetNum(InventorySlots.Num() + Amount);
		InventoryMenuWidget->AddSlots(Amount);
	}
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
	
}

void UInventoryComponent::Initialize()
{
	PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	PlayerContr = Cast<AHG_PlayerController>(GetOwner()->GetInstigatorController());

	InventorySlots.SetNum(PlayerContr->InventorySlots);

	if (InventoryMenuWidgetClass)
	{
		InventoryMenuWidget = CreateWidget<UInventoryMenuWidget>(PlayerContr, InventoryMenuWidgetClass);
		InventoryMenuWidget->AddToViewport();
		InventoryMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory menu widget class is missing"));
	}
	if (ExaminationWidgetClass)
	{
		ExaminationWidget = CreateWidget<UExaminationWidget>(PlayerContr, ExaminationWidgetClass);
		ExaminationWidget->InitializeWidget(this);
		PlayerCharacter->ExaminationWidget = ExaminationWidget;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Examination widget class is missing"));
	}
	if (ExaminationActorClass)
	{
		TArray<AActor*> outActors;
		UGameplayStatics::GetAllActorsOfClass(this, ExaminationActorClass, outActors);
		if (outActors.Num() > 0)
		{
			ExaminationActor = Cast<AExamination>(outActors[0]);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Examination actor class is missing"));
	}
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

//void UInventoryComponent::CreateExaminationWidget(int Index)
//{
//	ExaminationWidget->UpdateWidget(Index);
//	InventoryMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
//	ExaminationWidget->AddToViewport(2);
//}

