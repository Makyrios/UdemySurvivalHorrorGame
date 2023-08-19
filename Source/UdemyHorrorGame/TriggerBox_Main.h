// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "TriggerBox_Main.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYHORRORGAME_API ATriggerBox_Main : public ATriggerBox
{
	GENERATED_BODY()


protected:
	void BeginPlay() override;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
