// Fill out your copyright notice in the Description page of Project Settings.


#include "ProgressBarsWidget.h"
#include "Components/ProgressBar.h"
#include "PlayerCharacter.h"
#include "HealthComponent.h"
#include "MoveComponent.h"
#include "FlashlightComponent.h"

void UProgressBarsWidget::InitializeWidget(APlayerCharacter* PlayerCharacterRef)
{
	PlayerCharacter = PlayerCharacterRef;
	/*UHealthComponent* HealthComp = PlayerCharacter->GetHealthComponent();
	UMoveComponent* MoveComp = PlayerCharacter->GetMoveComponent();
	UFlashlightComponent* FlashlightComp = PlayerCharacter->GetFlashlightComponent();
	HealthBar->SetPercent((HealthComp->GetCurrentHealth() - HealthComp->MinHealth) / PlayerCharacter->GetHealthComponent()->MaxHealth);
	StaminaBar->SetPercent((MoveComp->GetCurrentStamina() - HealthComp->MinHealth) / PlayerCharacter->GetMoveComponent()->MaxStamina);
	BatteryBar->SetPercent((FlashlightComp->GetCurrentBatteryLevel() - FlashlightComp->MinBatteryLevel) / PlayerCharacter->GetFlashlightComponent()->MaxBatteryLevel);*/
}

float UProgressBarsWidget::BindHealthBar()
{
	UHealthComponent* HealthComp = PlayerCharacter->GetHealthComponent();
	return (HealthComp->GetCurrentHealth() - HealthComp->MinHealth) / PlayerCharacter->GetHealthComponent()->MaxHealth;
}

float UProgressBarsWidget::BindStaminaBar()
{
	UMoveComponent* MoveComp = PlayerCharacter->GetMoveComponent();
	return (MoveComp->GetCurrentStamina() - MoveComp->MinStamina) / PlayerCharacter->GetMoveComponent()->MaxStamina;
}

float UProgressBarsWidget::BindBatteryBar()
{
	UFlashlightComponent* FlashlightComp = PlayerCharacter->GetFlashlightComponent();
	return (FlashlightComp->GetCurrentBatteryLevel() - FlashlightComp->MinBatteryLevel) / PlayerCharacter->GetFlashlightComponent()->MaxBatteryLevel;
}


void UProgressBarsWidget::NativeConstruct()
{
	Super::NativeConstruct();

}
