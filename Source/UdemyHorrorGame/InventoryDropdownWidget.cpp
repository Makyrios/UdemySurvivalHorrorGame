// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryDropdownWidget.h"
#include "InventorySlotWidget.h"
#include "PlayerCharacter.h"
#include "InventoryComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Components/Button.h"

void UInventoryDropdownWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	UseButton->OnClicked.AddDynamic(this, &UInventoryDropdownWidget::UseItem);
	ExamineButton->OnClicked.AddDynamic(this, &UInventoryDropdownWidget::ExamineItem);
	DropButton->OnClicked.AddDynamic(this, &UInventoryDropdownWidget::DropItem);
}

void UInventoryDropdownWidget::UpdateMenu(UInventorySlotWidget* SlotWidget)
{
	if (SlotWidget == nullptr) return;

	SlotIndex = SlotWidget->GetIndex();

	if (PlayerCharacter->GetInventoryComponent()->GetItemClassAtIndex(SlotWidget->GetIndex()).GetDefaultObject()->GetItemData().bCanBeUsed)
	{
		UseButton->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		UseButton->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInventoryDropdownWidget::UseItem()
{
	PlayerCharacter->GetInventoryComponent()->UseItem(SlotIndex);
}

void UInventoryDropdownWidget::DropItem()
{
	PlayerCharacter->GetInventoryComponent()->DropItem(SlotIndex);
}

void UInventoryDropdownWidget::ExamineItem()
{
	PlayerCharacter->GetInventoryComponent()->ExamineItem(SlotIndex);
}
