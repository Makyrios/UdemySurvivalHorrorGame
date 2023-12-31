// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "HideActor.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnFinishedMoving)

UCLASS()
class UDEMYHORRORGAME_API AHideActor : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = Components)
	class UArrowComponent* PlayerPosition;
	UPROPERTY(VisibleAnywhere, Category = Components)
	class UArrowComponent* ExitPosition;
	UPROPERTY(VisibleAnywhere, Category = Components)
	class UArrowComponent* AIPosition;

	UPROPERTY(EditDefaultsOnly, Category = Locker)
	float InterpolationSpeed = 1000;

protected:
	UPROPERTY(VisibleAnywhere, Category = Components)
	UStaticMeshComponent* StaticMesh;

	FOnFinishedMoving OnFinishedMoving;
	
public:	
	// Sets default values for this actor's properties
	AHideActor();

	bool Interact() override;

	virtual void EnemyFound();

protected:
	bool bCanInteract = true;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveCharacter();

	UFUNCTION()
	void FinishedMoving();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	class APlayerCharacter* PlayerCharacter;
	class AClassicController* ClassicContr;


private:
	FTimerHandle MoveCharTimerHandle;

};
