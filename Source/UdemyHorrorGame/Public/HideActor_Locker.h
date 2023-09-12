// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HideActor.h"
#include "Components/TimeLineComponent.h"
#include "HideActor_Locker.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYHORRORGAME_API AHideActor_Locker : public AHideActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Interact)
	UCurveFloat* DoorCurveFloat;


protected:
	UPROPERTY(VisibleAnywhere, Category = Component)
	UStaticMeshComponent* DoorStaticMesh;

	void BeginPlay() override;

public:
	AHideActor_Locker();

	bool Interact() override;
	void EnemyFound() override;

private:
	UPROPERTY(VisibleAnywhere, Category = Components)
	UTimelineComponent* DoorTimeline;

	FOnTimelineFloat DoorTimelineFloat;
	
	UFUNCTION()
	void OpenDoor(float Value);
	
	void OnFinishedMove_Event();

};
