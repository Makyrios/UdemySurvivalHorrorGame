// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindRandomLocationNearKey.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYHORRORGAME_API UBTTask_FindRandomLocationNearKey : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float SearchRadius = 200;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
