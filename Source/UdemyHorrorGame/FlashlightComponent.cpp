// Fill out your copyright notice in the Description page of Project Settings.


#include "FlashlightComponent.h"
#include "Components/SpotLightComponent.h"
#include "PlayerCharacter.h"

// Sets default values for this component's properties
UFlashlightComponent::UFlashlightComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UFlashlightComponent::Initialize(APlayerCharacter* PlayerCharRef)
{
	PlayerCharacter = PlayerCharRef;
	FlashlightLight = PlayerCharacter->GetSpotlightComponent();
}

void UFlashlightComponent::ToggleFlashlight()
{
	if (CurrentBatteryLevel > 0)
	{
		// Turn on
		if (!FlashlightLight->IsVisible())
		{
			FlashlightLight->SetVisibility(true);
			GetWorld()->GetTimerManager().SetTimer(DepletionTimerHandle, this, &UFlashlightComponent::DepleteBatteryLife, 0.2, true);
		}
		// Turn off
		else
		{
			FlashlightLight->SetVisibility(false);
			GetWorld()->GetTimerManager().ClearTimer(DepletionTimerHandle);
		}
	}
}

void UFlashlightComponent::SetIntensity()
{
	TRange<float> BatteryLife {MinBatteryLevel, FadingLightThreshold};
	TRange<float> IntensityRange {0, 1};
	FlashlightLight->SetIESBrightnessScale(FMath::GetMappedRangeValueClamped<float>(BatteryLife, IntensityRange, CurrentBatteryLevel));
}

void UFlashlightComponent::FlickerLight()
{
	FlashlightLight->LightFunctionMaterial = FlickeringLightMaterial;
}

void UFlashlightComponent::StopFlickerLight()
{
	FlashlightLight->ClearLightFunctionMaterial();
}

void UFlashlightComponent::AddBatteryLife(int Amount)
{
	CurrentBatteryLevel = FMath::Clamp(CurrentBatteryLevel + Amount, MinBatteryLevel, MaxBatteryLevel);
	if (CurrentBatteryLevel > FlickeringLightThreshold)
	{
		StopFlickerLight();
	}
}

void UFlashlightComponent::DepleteBatteryLife()
{
	CurrentBatteryLevel = FMath::Clamp(CurrentBatteryLevel - DepletionAmount, MinBatteryLevel, MaxBatteryLevel);
	if (CurrentBatteryLevel <= 0.1)
	{
		FlashlightLight->SetVisibility(false);
		GetWorld()->GetTimerManager().ClearTimer(DepletionTimerHandle);
	}
	// Fading light

	SetIntensity();

	if (CurrentBatteryLevel <= FlickeringLightThreshold)
	{
		FlickerLight();
	}
}


// Called when the game starts
void UFlashlightComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFlashlightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UE_LOG(LogTemp, Display, TEXT("Battery life: %f"), CurrentBatteryLevel);
}

