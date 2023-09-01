// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ObjectiveData.h"
#include "HG_GameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYHORRORGAME_API AHG_GameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = Objectives)
	TArray<FObjectiveData> Objectives;

public:
	void Initialize(class UMainHUDWidget* MainHUDRef);

	/// <summary>
	/// Call to complete current objective, and if bool is true, set it to the next
	/// </summary>
	/// <param name="ObjectiveID">ID of objective to complete</param>
	/// <param name="bSetNextObjectiveImmediately">Immediately set next objective or not</param>
	UFUNCTION(BlueprintCallable)
	void CompleteObjectiveCall(FName ObjectiveID, bool bSetNextObjectiveImmediately);
	
	/// <summary>
	/// Set next objective
	/// </summary>
	void SetObjective();

private:
	class UMainHUDWidget* MainHUD;

	FObjectiveData FindCurrentObjective();

	void CompleteObjective();
};
