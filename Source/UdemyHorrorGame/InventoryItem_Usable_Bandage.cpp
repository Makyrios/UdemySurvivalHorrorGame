// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem_Usable_Bandage.h"

bool AInventoryItem_Usable_Bandage::UseItem()
{
	Super::UseItem();
	// Implement healing
	UE_LOG(LogTemp, Display, TEXT("Heal for %f HP"), HealAmount);
	return true;
}
