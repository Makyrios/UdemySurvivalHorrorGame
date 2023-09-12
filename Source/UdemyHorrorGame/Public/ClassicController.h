// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include <Perception/AIPerceptionTypes.h>
#include "PlayerHiding.h"
#include "ClassicController.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYHORRORGAME_API AClassicController : public AAIController, public IPlayerHiding
{
	GENERATED_BODY()

	//UPROPERTY(VisibleAnywhere, Category = Components)
	//class UAIPerceptionComponent* PerceptionComponent;

	

public:
	AClassicController();

	void PullOutOfHiding();

	// Inherited via IPlayerHiding
	virtual void DidEnemySee(AHideActor* HideActor) override;
	virtual void LeftHidingSpot() override;

private:
	void OnPossess(APawn* InPawn) override;
	void OnUnPossess() override;
	
private:
	UPROPERTY(transient)
	class UBlackboardComponent* BlackboardComp;

	UPROPERTY(Transient)
	class UBehaviorTreeComponent* BehaviorComp;

	class AHideActor* LastHideActor;

	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);



};
