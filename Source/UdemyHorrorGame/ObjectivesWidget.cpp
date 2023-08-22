// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectivesWidget.h"
#include "Components/TextBlock.h"

void UObjectivesWidget::UpdateObjectiveText(FText ObjectiveText)
{
	UE_LOG(LogTemp, Warning, TEXT("Text: %s"), *ObjectiveText.ToString());
	ObjectiveTitleText->SetText(ObjectiveText); 
}
