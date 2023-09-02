// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include <Perception/AIPerceptionTypes.h>
#include "ClassicController.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYHORRORGAME_API AClassicController : public AAIController
{
	GENERATED_BODY()

	//UPROPERTY(VisibleAnywhere, Category = Components)
	//class UAIPerceptionComponent* PerceptionComponent;

public:
	AClassicController();

private:
	void OnPossess(APawn* InPawn) override;
	void OnUnPossess() override;
	
private:
	UPROPERTY(transient)
	class UBlackboardComponent* BlackboardComp;

	UPROPERTY(Transient)
	class UBehaviorTreeComponent* BehaviorComp;


	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

};
