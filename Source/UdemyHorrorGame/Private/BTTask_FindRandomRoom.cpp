

// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindRandomRoom.h"
#include <ClassicController.h>
#include <Classic.h>
#include "BehaviorTree/BlackBoardComponent.h"
#include <NavigationSystem.h>
#include <AITargetPoint.h>

EBTNodeResult::Type UBTTask_FindRandomRoom::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AClassicController* MyController = Cast<AClassicController>(OwnerComp.GetAIOwner());
	if (MyController == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	AClassic* AIChar = Cast<AClassic>(MyController->GetPawn());
	if (AIChar->GetRoomTargetPoints().Num() > 0)
	{
		int RandomRoom = FMath::RandRange(0, AIChar->GetRoomTargetPoints().Num() - 1);
		AAITargetPoint* TargetPoint = AIChar->GetRoomTargetPoints()[RandomRoom];
		FVector RandomLocation;
		UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(this, TargetPoint->GetActorLocation(), RandomLocation, TargetPoint->GetSphereRadius());
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("targetLocation"), RandomLocation);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
