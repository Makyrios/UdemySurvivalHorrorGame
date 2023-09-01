// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TriggerBox_Main.h"
#include "TriggerBox_Jumpscare.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYHORRORGAME_API ATriggerBox_Jumpscare : public ATriggerBox_Main
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Sound)
	class USoundCue* JumpscareCue;

protected:
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
};
