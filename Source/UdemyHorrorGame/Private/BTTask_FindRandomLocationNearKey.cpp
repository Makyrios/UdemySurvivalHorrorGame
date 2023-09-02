// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindRandomLocationNearKey.h"
#include <ClassicController.h>
#include <Classic.h>
#include <NavigationSystem.h>
#include "BehaviorTree/BlackBoardComponent.h"


EBTNodeResult::Type UBTTask_FindRandomLocationNearKey::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FVector RandomLocation;
	FVector OriginLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
	UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(this, OriginLocation, RandomLocation, SearchRadius);
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("targetLocation"), RandomLocation);

	return EBTNodeResult::Succeeded;
}
