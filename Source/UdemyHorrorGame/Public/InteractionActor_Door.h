// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionActor.h"
#include "Components/TimelineComponent.h"
#include "InteractionActor_Door.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYHORRORGAME_API AInteractionActor_Door : public AInteractionActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Timeline)
	UCurveFloat* DoorCurveFloat;

public:
	AInteractionActor_Door();

protected:
	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* DoorStaticMesh;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	bool Interact() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = Timeline)
	UTimelineComponent* TimelineComponent;

	FOnTimelineFloat DoorTimelineFloat;
	bool bIsOpened = false;

	UFUNCTION()
	virtual void OpenDoor(float Value);
};
