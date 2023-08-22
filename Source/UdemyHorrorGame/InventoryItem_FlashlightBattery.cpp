// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem_FlashlightBattery.h"
#include "FlashlightComponent.h"
#include "HG_GameStateBase.h"
#include <Kismet/GameplayStatics.h>

bool AInventoryItem_FlashlightBattery::UseItem()
{
	UE_LOG(LogTemp, Display, TEXT("Flashlight battery use"));
	UFlashlightComponent* FlashlightComp = PlayerCharacter->GetFlashlightComponent();
	if (FlashlightComp->GetCurrentBatteryLevel() < FlashlightComp->MaxBatteryLevel)
	{
		FlashlightComp->AddBatteryLife(BatteryRegenAmount); 
		Cast<AHG_GameStateBase>(UGameplayStatics::GetGameState(this))->CompleteObjectiveCall(FName("UseBattery"), true);
		return true;
	}
	return false;
}
