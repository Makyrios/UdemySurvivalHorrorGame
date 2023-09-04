// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckAttack.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include <Kismet/GameplayStatics.h>
#include "AIController.h"
#include "GameFramework/Character.h"


void UBTService_CheckAttack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (Player == nullptr || OwnerComp.GetAIOwner() == nullptr || OwnerComp.GetAIOwner()->GetPawn() == nullptr)
	{
		return;
	}
	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	
	if (FVector::Distance(ControlledPawn->GetActorLocation(), Player->GetActorLocation()) < AttackDistance)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("canAttackPlayer"), true);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("canAttackPlayer"), false);
	}
}
