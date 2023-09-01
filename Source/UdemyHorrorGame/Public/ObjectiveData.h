// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectiveData.generated.h"

USTRUCT(BlueprintType)
struct FObjectiveData
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere)
	FName IDName;
	UPROPERTY(EditAnywhere)
	FText ObjectiveTitle;
	UPROPERTY(EditAnywhere)
	bool bObjectiveCompleted;

	friend bool operator==(const FObjectiveData& lhs, const FObjectiveData& rhs);

};