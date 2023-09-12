// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassicController.h"
#include <Classic.h>
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include <Perception/AISense_Sight.h>
#include <Perception/AISense_Hearing.h>
#include "HideActor.h"
#include <Kismet/GameplayStatics.h>
#include "PlayerCharacter.h"
#include "Components/ArrowComponent.h"


AClassicController::AClassicController()
{
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree Component"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AClassicController::OnPerceptionUpdated);

}

void AClassicController::PullOutOfHiding()
{
	LastHideActor->EnemyFound();
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
	UE_LOG(LogTemp, Display, TEXT("Perception update"));
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
			APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Actor);
			if (PlayerCharacter != nullptr && !PlayerCharacter->bIsHiding)
			{
				BlackboardComp->ClearValue(FName("investigateLocation"));
				BlackboardComp->SetValueAsObject(FName("playerActor"), Actor);
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("Out of sight"));
				BlackboardComp->SetValueAsVector(FName("investigateLocation"), Stimulus.StimulusLocation);
				DrawDebugSphere(GetWorld(), Stimulus.StimulusLocation, 50, 20, FColor::Red, true);
				BlackboardComp->ClearValue(FName("playerActor"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Out of sight"));
			BlackboardComp->SetValueAsVector(FName("investigateLocation"), Stimulus.StimulusLocation);
			DrawDebugSphere(GetWorld(), Stimulus.StimulusLocation, 50, 20, FColor::Red, true);
			BlackboardComp->ClearValue(FName("playerActor"));
		}
	}
	else if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			BlackboardComp->SetValueAsVector(FName("noiseLocation"), Stimulus.StimulusLocation);
		}
	}
}

void AClassicController::LeftHidingSpot()
{
	GetBlackboardComponent()->SetValueAsBool(FName("killHiddenPlayer"), false);
}

void AClassicController::DidEnemySee(AHideActor* HideActor)
{
	LastHideActor = HideActor;
	AActor* Player = Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FName("playerActor")));
	if (Player != nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Did see player"));
		GetBlackboardComponent()->SetValueAsBool(FName("killHiddenPlayer"), true);
		GetBlackboardComponent()->SetValueAsVector(FName("targetLocation"), HideActor->AIPosition->GetComponentLocation());
		GetBlackboardComponent()->SetValueAsRotator(FName("targetRotation"), HideActor->AIPosition->GetComponentRotation());
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Did not see player"));
		GetBlackboardComponent()->SetValueAsBool(FName("killHiddenPlayer"), false);
	}
}
