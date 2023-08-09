// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem_Usable_Bandage.h"
#include "HealthComponent.h"

bool AInventoryItem_Usable_Bandage::UseItem()
{
	Super::UseItem();
	// Implement healing
	UE_LOG(LogTemp, Display, TEXT("Heal for %f HP"), HealAmount);
	UHealthComponent* HealthComponent = PlayerCharacter->GetHealthComponent();
	if (HealthComponent->CurrentHealth < HealthComponent->MaxHealth)
	{
		HealthComponent->ChangeHealth(HealAmount);
		return true;
	}
	return false;
}
