// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HG_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYHORRORGAME_API AHG_PlayerController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = HUD)
	TSubclassOf<UUserWidget> GameOverWidget;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	int InventorySlots = 8;
	
	void GameOver();

private:
	void OnPossess(APawn* PossessedPawn) override;
};
