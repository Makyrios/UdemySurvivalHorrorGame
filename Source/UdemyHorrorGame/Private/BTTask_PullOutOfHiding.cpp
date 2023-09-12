// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PullOutOfHiding.h"
#include <ClassicController.h>

EBTNodeResult::Type UBTTask_PullOutOfHiding::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (OwnerComp.GetAIOwner() != nullptr)
	{
		AClassicController* Contr = Cast<AClassicController>(OwnerComp.GetAIOwner());
		if (Contr != nullptr)
		{
			Contr->PullOutOfHiding();
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
