// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionActor_LightSwitch.h"
#include "Engine/Light.h"
#include "Components/LightComponent.h"

bool AInteractionActor_LightSwitch::Interact()
{
	Super::Interact();

	if (LightRef == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Light ref is null for %s"), *GetActorNameOrLabel());
		return false;
	}

	if (LightRef->GetLightComponent()->IsVisible())
	{
		LightRef->GetLightComponent()->SetVisibility(false);
	}
	else
	{
		LightRef->GetLightComponent()->SetVisibility(true);
	}
	return true;
}
