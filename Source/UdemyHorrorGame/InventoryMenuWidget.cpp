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
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"

void UInventoryMenuWidget::OpenDropdownMenu(UInventorySlotWidget* SlotWidget)
{
	if (SlotWidget == nullptr) return;

	UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(SlotWidget->Slot);
	if (GridSlot == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not cast grid slot"));
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

void UInventoryMenuWidget::ShowItemDetails(UInventorySlotWidget* SlotWidget)
{
	if (SlotWidget == nullptr) return;

	FItemData ItemData;
	UE_LOG(LogTemp, Display, TEXT("Index: %d"), SlotWidget->GetIndex());
	AInventoryItem_Main* ItemObject = PlayerCharacter->GetInventoryComponent()->GetItemClassAtIndex(SlotWidget->GetIndex()).GetDefaultObject();
	if (ItemObject != nullptr)
	{
		ItemData = ItemObject->GetItemData();
		NameText->SetText(FText::FromName(ItemData.ItemName));
		DescriptionText->SetText(FText::FromName(ItemData.Description));
	}
	DetailsVerticalBox->SetVisibility(ESlateVisibility::Visible);
}

void UInventoryMenuWidget::HideItemDetails()
{
	DetailsVerticalBox->SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryMenuWidget::AddSlots(int SlotsAmount)
{
	InventoryGrid->AddSlots(SlotsAmount);
	int SlotsArrayNum = InventoryGrid->GetSlotsArray().Num();
	for (int i = SlotsArrayNum - 1; i >= SlotsArrayNum - SlotsAmount; i--)
	{
		InitializeSlotWidget(i);
	}
}

void UInventoryMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	CloseDropdownMenuButton->OnClicked.AddDynamic(this, &UInventoryMenuWidget::CloseDropdownMenu);

	for (int i = 0; i < InventoryGrid->GetSlotsArray().Num(); i++)
	{
		InitializeSlotWidget(i);
	}
}

void UInventoryMenuWidget::InitializeSlotWidget(int i)
{
	InventoryGrid->GetSlotsArray()[i]->InitializeWidget(i, this);
}
