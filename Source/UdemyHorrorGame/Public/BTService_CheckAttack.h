// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckAttack.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYHORRORGAME_API UBTService_CheckAttack : public UBTService
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float AttackDistance = 50;

private:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
