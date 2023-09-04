// Fill out your copyright notice in the Description page of Project Settings.


#include "HG_PlayerController.h"
#include "Blueprint/UserWidget.h"

void AHG_PlayerController::GameOver()
{
	if (GameOverWidget != nullptr)
	{
		UUserWidget* GameOver = CreateWidget(this, GameOverWidget);
		GameOver->AddToViewport();
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameOver widget is null"));
	}
}

void AHG_PlayerController::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);

	SetInputMode(FInputModeGameOnly());
}
