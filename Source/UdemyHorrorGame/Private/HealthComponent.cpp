// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollection.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	BloodScreenMaterialCollection = LoadObject<UMaterialParameterCollection>(NULL, TEXT("/Script/Engine.MaterialParameterCollection'/Game/Materials/MPC_BloodScreen.MPC_BloodScreen'"));
}

void UHealthComponent::ChangeHealth(float Amount, AActor* DamageCauser)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, MinHealth, MaxHealth);
	UpdateBloodScreen();
	if (CurrentHealth <= 0)
	{
		if (OnPlayerDieEvent.IsBound())
		{
			OnPlayerDieEvent.Broadcast(DamageCauser);
		}
	}
}

void UHealthComponent::UpdateBloodScreen()
{
	TRange<float> HealthRange {MinHealth, MaxHealth * BloodScreenActivationPercent};
	TRange<float> BloodAmountRange {0.5, 1};
	float Amount = FMath::GetMappedRangeValueClamped(HealthRange, BloodAmountRange, CurrentHealth);
	UKismetMaterialLibrary::SetScalarParameterValue(this, BloodScreenMaterialCollection, FName("Amount"), Amount);
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

