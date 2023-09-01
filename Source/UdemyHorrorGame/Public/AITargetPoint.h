// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "Components/SphereComponent.h"
#include "AITargetPoint.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYHORRORGAME_API AAITargetPoint : public ATargetPoint
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = Components)
	class USphereComponent* Sphere;

public:
	AAITargetPoint();

	inline float GetSphereRadius() const { return Sphere->GetScaledSphereRadius(); }
	
};
