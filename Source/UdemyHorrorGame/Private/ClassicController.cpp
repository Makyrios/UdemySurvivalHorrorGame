// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassicController.h"
#include <Classic.h>

void AClassicController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AClassic* Bot = Cast<AClassic>(InPawn))
	{
		BehaviorTree = Bot->BotBehavior;
	}
	RunBehaviorTree(BehaviorTree);
}
