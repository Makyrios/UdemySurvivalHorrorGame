// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"
#include <Kismet/GameplayStatics.h>
#include "PlayerCharacter.h"
#include "InventoryComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"


void UInventorySlotWidget::UpdateSlot()
{
	int Amount = 0;
	AInventoryItem_Main* Item = PlayerCharacter->GetInventoryComponent()->GetItemClassAtIndex(Index, &Amount).GetDefaultObject();
	if (Item == nullptr)
	{
		SlotButton->SetIsEnabled(false);
		SlotImage->SetBrushFromTexture(EmptyIcon, true);
		AmountText->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		FItemData Data = Item->GetItemData();
		SlotButton->SetIsEnabled(true);
		SlotImage->SetBrushFromTexture(Data.Icon, true);
		if (Amount > 0)
		{
			AmountText->SetText(FText::AsNumber(Amount));
			AmountText->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	Super::NativeOnMouseButtonDown(Geometry, MouseEvent);

	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		LastPressedButton = EKeys::LeftMouseButton;
	}
	else if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		LastPressedButton = EKeys::RightMouseButton;
	}
	return FReply::Handled();
}

FReply UInventorySlotWidget::NativeOnMouseButtonUp(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	Super::NativeOnMouseButtonUp(Geometry, MouseEvent);

	if (LastPressedButton == EKeys::LeftMouseButton)
	{
		// Item info
	}
	else if (LastPressedButton == EKeys::RightMouseButton)
	{
		InventoryMenu->OpenDropdownMenu(this);
	}


	return FReply::Handled();
}

void UInventorySlotWidget::InitializeWidget(int ind, UInventoryMenuWidget* InventoryMenuRef)
{
	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	InventoryMenu = InventoryMenuRef;
	Index = ind;
	UpdateSlot();
}
