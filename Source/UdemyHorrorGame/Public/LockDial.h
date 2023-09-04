// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include <Components/TimelineComponent.h>
#include "LockDial.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDialRoll)

UCLASS()
class UDEMYHORRORGAME_API ALockDial : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = Components)
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, Category = Components)
	UStaticMeshComponent* DialMesh;

	UPROPERTY(VisibleAnywhere, Category = Components)
	UTimelineComponent* RotateTimelineComponent;

	UPROPERTY(VisibleAnywhere, Category = Components)
	class UWidgetComponent* TopArrowComponent;
	UPROPERTY(VisibleAnywhere, Category = Components)
	class UWidgetComponent* BottomArrowComponent;

	/*UPROPERTY(VisibleAnywhere, Category = Initialization)
	TSubclassOf<UUserWidget> ArrowWidgetClass;*/

public:
	FOnDialRoll OnDialRoll;

public:
	// Sets default values for this actor's properties
	ALockDial();

	void Initialize(int ind);

	UFUNCTION(BlueprintCallable)
	void RollNumberUp();
	UFUNCTION(BlueprintCallable)
	void RollNumberDown();

	void ToggleArrows();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	inline int GetNumber() const { return Number; }

private:
	UPROPERTY(EditDefaultsOnly, Category = Timeline)
	UCurveFloat* RotateCurveFloat;

	UPROPERTY(EditAnywhere, Category = Lock)
	bool bRandomDial = true;

	int Index;
	int Number = 0;
	float DialRotation;
	float RotationRate;

	bool bCanRotate = true;
	bool bIsForward = true;

	FOnTimelineFloat OnTimelineFloat;

	UFUNCTION()
	void Rotate(float Value);
	UFUNCTION()
	void FinishRotate();

	/// <summary>
	/// Find approriate number according to the roll of dial
	/// </summary>
	/// <param name="Roll">Roll 0-360 degrees</param>
	/// <returns>Number 0-9</returns>
	int FindNumber(float Roll) const;
};
