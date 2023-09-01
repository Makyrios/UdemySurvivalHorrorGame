// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ClassicController.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYHORRORGAME_API AClassicController : public AAIController
{
	GENERATED_BODY()
	
	UPROPERTY(Transient)
	class UBehaviorTree* BehaviorTree;

	void OnPossess(APawn* InPawn) override;
};
