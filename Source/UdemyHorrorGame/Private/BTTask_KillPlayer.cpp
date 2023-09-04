// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_KillPlayer.h"
#include <Classic.h>
#include <Kismet/GameplayStatics.h>
#include "AIController.h"
#include "PlayerCharacter.h"

EBTNodeResult::Type UBTTask_KillPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (Player == nullptr || OwnerComp.GetAIOwner() == nullptr || OwnerComp.GetAIOwner()->GetPawn() == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	if (AClassic* OwnedClassicAI = Cast<AClassic>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		OwnedClassicAI->CatchTarget(Player);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
