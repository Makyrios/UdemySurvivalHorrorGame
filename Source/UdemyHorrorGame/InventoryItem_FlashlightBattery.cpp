// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem_FlashlightBattery.h"
#include "FlashlightComponent.h"

bool AInventoryItem_FlashlightBattery::UseItem()
{
	UE_LOG(LogTemp, Display, TEXT("Flashlight battery use"));
	UFlashlightComponent* FlashlightComp = PlayerCharacter->GetFlashlightComponent();
	if (FlashlightComp->CurrentBatteryLevel < FlashlightComp->MaxBatteryLevel)
	{
		FlashlightComp->AddBatteryLife(BatteryRegenAmount); 
		return true;
	}
	return false;
}
