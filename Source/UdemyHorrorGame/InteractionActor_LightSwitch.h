// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionActor.h"
#include "InteractionActor_LightSwitch.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYHORRORGAME_API AInteractionActor_LightSwitch : public AInteractionActor
{
	GENERATED_BODY()
	
public:
	void Interact() override;

private:
	UPROPERTY(EditInstanceOnly, Category = Light)
	class ALight* LightRef;

};
