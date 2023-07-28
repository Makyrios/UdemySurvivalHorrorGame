// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionActor_Door.h"
#include "InteractionActor_OutwardsDoor.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYHORRORGAME_API AInteractionActor_OutwardsDoor : public AInteractionActor_Door
{
	GENERATED_BODY()

public:
	AInteractionActor_OutwardsDoor();

protected:
	void BeginPlay() override;

	void Tick(float DeltaTime) override;

	void OpenDoor(float Value) override;

public:
	void Interact() override;

private:
	float DotProduct;

	ACharacter* Character;

	
};
