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
	FItemData Data = PlayerCharacter->GetInventoryComponent()->GetItemDataAtIndex(Index, &Amount);
	if (Data.ItemName.IsNone())
	{
		UE_LOG(LogTemp, Display, TEXT("None name %s"), *Data.ItemName.ToString());
		SlotButton->SetIsEnabled(false);
		SlotImage->SetBrushFromTexture(EmptyIcon, true);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Is name %s"), *Data.ItemName.ToString());
		SlotButton->SetIsEnabled(true);
		SlotImage->SetBrushFromTexture(Data.Icon, true);
		if (Amount > 0)
		{
			AmountText->SetText(FText::AsNumber(Amount));
			AmountText->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			AmountText->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UInventorySlotWidget::InitializeWidget(int ind)
{
	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	Index = ind;
	UpdateSlot();
}
