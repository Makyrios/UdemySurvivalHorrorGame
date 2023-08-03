// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryMenuWidget.h"
#include "InventoryGridWidget.h"
#include "InventorySlotWidget.h"
#include "InventoryDropdownWidget.h"
#include "Components/UniformGridSlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include <Kismet/GameplayStatics.h>
#include "PlayerCharacter.h"
#include "InventoryComponent.h"

void UInventoryMenuWidget::OpenDropdownMenu(UInventorySlotWidget* SlotWidget)
{
	if (SlotWidget == nullptr) return;

	UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(SlotWidget->Slot);
	if (GridSlot == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No cast"));
		return;
	}
	if (!PlayerCharacter->GetInventoryComponent()->GetItemClassAtIndex(SlotWidget->GetIndex()).GetDefaultObject())
	{
		return;
	}
	int Row = GridSlot->GetRow() + 1;
	int Col = GridSlot->GetColumn() + 1;
	float XPadding = InventoryGrid->InventoryGridPanel->GetSlotPadding().Left + InventoryGrid->InventoryGridPanel->GetSlotPadding().Right;
	float YPadding = InventoryGrid->InventoryGridPanel->GetSlotPadding().Top + InventoryGrid->InventoryGridPanel->GetSlotPadding().Bottom;
	int X = FMath::TruncToInt((SlotWidget->GetDesiredSize().X + XPadding)) * Col;
	int Y = FMath::TruncToInt((SlotWidget->GetDesiredSize().Y + YPadding)) * Row;

	FVector2D NewTranslation(X, Y);
	DropdownWidget->SetRenderTranslation(NewTranslation);
	CloseDropdownMenuButton->SetVisibility(ESlateVisibility::Visible);
	DropdownWidget->UpdateMenu(SlotWidget);
	DropdownWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UInventoryMenuWidget::CloseDropdownMenu()
{
	DropdownWidget->SetVisibility(ESlateVisibility::Collapsed);
	CloseDropdownMenuButton->SetVisibility(ESlateVisibility::Collapsed);
}

void UInventoryMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CloseDropdownMenuButton->OnClicked.AddDynamic(this, &UInventoryMenuWidget::CloseDropdownMenu);
	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	for (int i = 0; i < InventoryGrid->GetSlotsArray().Num(); i++)
	{
		InventoryGrid->GetSlotsArray()[i]->InitializeWidget(i, this);
	}
}
