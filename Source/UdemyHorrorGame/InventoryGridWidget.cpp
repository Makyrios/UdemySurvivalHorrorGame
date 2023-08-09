// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryGridWidget.h"
#include "HG_PlayerController.h"
#include <Kismet/GameplayStatics.h>
#include "Components/UniformGridPanel.h"
#include "PlayerCharacter.h"
#include "InventoryComponent.h"


void UInventoryGridWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerContr = Cast<AHG_PlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (!SlotWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("No inventory slot widget class"));
		return;
	}

	for (int i = 0; i < PlayerContr->InventorySlots; i++)
	{
		CreateSlotWidget(i);
	}
}

void UInventoryGridWidget::CreateSlotWidget(int i)
{
	UInventorySlotWidget* SlotWidget = CreateWidget<UInventorySlotWidget>(PlayerContr, SlotWidgetClass);
	SlotsArray.Add(SlotWidget);

	int Row = i / SlotsPerRow;
	int Col = i % SlotsPerRow;

	if (SlotWidget)
	{
		if (InventoryGridPanel)
		{
			InventoryGridPanel->AddChildToUniformGrid(SlotWidget, Row, Col);
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Inventory grid panel null"));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Slot widget null"));
	}
}


void UInventoryGridWidget::AddSlots(int Amount)
{
	int OldSlotsAmount = SlotsArray.Num();
	int NewSlotsAmount = PlayerCharacter->GetInventoryComponent()->GetInventorySlots().Num();

	for (int i = OldSlotsAmount; i < NewSlotsAmount; i++)
	{
		CreateSlotWidget(i);
	}
}