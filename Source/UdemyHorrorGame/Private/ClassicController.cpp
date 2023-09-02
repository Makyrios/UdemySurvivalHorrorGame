// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassicController.h"
#include <Classic.h>
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include <Perception/AISense_Sight.h>

AClassicController::AClassicController()
{
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree Component"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AClassicController::OnPerceptionUpdated);
}

void AClassicController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AClassic* Bot = Cast<AClassic>(InPawn))
	{
		if (Bot->BotBehavior)
		{
			BlackboardComp->InitializeBlackboard(*(Bot->BotBehavior->BlackboardAsset));
			BehaviorComp->StartTree(*(Bot->BotBehavior));
		}
	}
	
	BlackboardComp->SetValueAsBool(FName("playJumpscare"), true);
}

void AClassicController::OnUnPossess()
{
	Super::OnUnPossess();

	BehaviorComp->StopTree();
}

void AClassicController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Actor == nullptr)
	{
		BlackboardComp->SetValueAsObject(FName("playerActor"), nullptr);
		return;
	}
	// Sight sense
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			BlackboardComp->ClearValue(FName("investigateLocation"));
			BlackboardComp->SetValueAsObject(FName("playerActor"), Actor);
		}
		else
		{
			BlackboardComp->SetValueAsVector(FName("investigateLocation"), Stimulus.StimulusLocation);
			DrawDebugSphere(GetWorld(), Stimulus.StimulusLocation, 50, 20, FColor::Red, true);
			BlackboardComp->ClearValue(FName("playerActor"));
		}
	}
}
