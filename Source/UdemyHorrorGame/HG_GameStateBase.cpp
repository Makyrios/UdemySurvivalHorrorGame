// Fill out your copyright notice in the Description page of Project Settings.


#include "HG_GameStateBase.h"
#include "MainHUDWidget.h"
#include "ObjectivesWidget.h"

void AHG_GameStateBase::Initialize(UMainHUDWidget* MainHUDRef)
{
	MainHUD = MainHUDRef;
	if (MainHUD == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Main HUD is null in GameState"));
		return;
	}
	SetObjective();
}

FObjectiveData AHG_GameStateBase::FindCurrentObjective()
{
	for (auto& Objective : Objectives)
	{
		if (!Objective.bObjectiveCompleted)
		{
			return Objective;
		}
	}
	return FObjectiveData();
}

void AHG_GameStateBase::CompleteObjectiveCall(FName ObjectiveID, bool bSetNextObjectiveImmediately)
{
	if (FindCurrentObjective().IDName == ObjectiveID)
	{
		CompleteObjective();
		if (bSetNextObjectiveImmediately)
		{
			SetObjective();
		}
	}
}

void AHG_GameStateBase::SetObjective()
{
	FObjectiveData CurrentObjective = FindCurrentObjective();
	MainHUD->ObjectivesWidget->UpdateObjectiveText(CurrentObjective.ObjectiveTitle);
	MainHUD->ObjectivesWidget->SetVisibility(ESlateVisibility::Visible);
}

void AHG_GameStateBase::CompleteObjective()
{
	FObjectiveData CurrentObjective = FindCurrentObjective();
	int FoundObjective = Objectives.Find(CurrentObjective);
	if (FoundObjective != INDEX_NONE)
	{
		Objectives[FoundObjective].bObjectiveCompleted = true;
		MainHUD->ObjectivesWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
