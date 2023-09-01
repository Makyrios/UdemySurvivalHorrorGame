// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObjectivesWidget.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYHORRORGAME_API UObjectivesWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* Canvas;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USizeBox* ObjectiveTitleSizeBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ObjectiveTitleText;

public:
	void UpdateObjectiveText(FText ObjectiveText);

};
