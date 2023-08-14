// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Lock.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnEnterLockView)
DECLARE_MULTICAST_DELEGATE(FOnExitLockView)

UCLASS()
class UDEMYHORRORGAME_API ALock : public AActor, public IInteractable
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = Components)
	USkeletalMeshComponent* LockMesh;
	UPROPERTY(VisibleAnywhere, Category = Components)
	class UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, Category = Components)
	class UBoxComponent* Box;
	UPROPERTY(VisibleAnywhere, Category = Components)
	class UWidgetInteractionComponent* WidgetInteraction;

	UPROPERTY(EditDefaultsOnly, Category = Initialization)
	TSubclassOf<class ALockDial> LockDialClass;

	UPROPERTY(EditAnywhere, Category = Initialization)
	TArray<int> LockCombination;

	UPROPERTY(EditAnywhere, Category = Initialization)
	FVector BoxPosition;

	UPROPERTY(EditAnywhere, Category = Initialization)
	FVector BoxExtent;

	UPROPERTY(EditDefaultsOnly, Category = Animations)
	UAnimationAsset* UnlockAnimation;
	
public:
	FOnEnterLockView OnEnterLockView;
	FOnExitLockView OnExitLockView;

	// Sets default values for this actor's properties
	ALock();

	void OnConstruction(const FTransform& Transform) override;

	void Interact() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TArray<class ALockDial*> Dials;

	bool bIsUnlocked = false;

	class APlayerCharacter* PlayerCharacter;
	class APlayerController* PlayerContr;

	void Initialize();
	void CreateDials();

	void OnDialRoll();
	bool CheckCode() const;

	void EnterLockView();
	void ExitLockView();

	void MouseClickStarted();
	void MouseClickCompleted();
};
