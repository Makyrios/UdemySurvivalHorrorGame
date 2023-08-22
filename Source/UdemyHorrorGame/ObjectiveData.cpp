// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectiveData.h"

bool operator==(const FObjectiveData& lhs, const FObjectiveData& rhs)
{
	return lhs.IDName == rhs.IDName;
}
