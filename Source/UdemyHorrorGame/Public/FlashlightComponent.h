// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FlashlightComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UDEMYHORRORGAME_API UFlashlightComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = Components)
	class USpotLightComponent* FlashlightLight;

public:
	UPROPERTY(EditAnywhere, Category = Flashlight)
	float MaxBatteryLevel = 100;
	UPROPERTY(EditAnywhere, Category = Flashlight)
	float MinBatteryLevel = 0;

	UPROPERTY(EditAnywhere, Category = Flashlight)
	float DepletionRate = 0.2;
	UPROPERTY(EditAnywhere, Category = Flashlight)
	float DepletionAmount = 1;

	UPROPERTY(EditAnywhere, Category = Flashlight)
	float FadingLightThreshold = 20;

	UPROPERTY(EditAnywhere, Category = Flashlight)
	float FlickeringLightThreshold = 10;
	UPROPERTY(EditAnywhere, Category = Flashlight)
	TObjectPtr<UMaterialInterface> FlickeringLightMaterial;

public:	
	// Sets default values for this component's properties
	UFlashlightComponent();
	
	void Initialize(class APlayerCharacter* PlayerCharRef);

	void ToggleFlashlight();


	void AddBatteryLife(int Amount);
	void DepleteBatteryLife();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GetCurrentBatteryLevel() const;

public:

private:
	float CurrentBatteryLevel = MaxBatteryLevel;

	FTimerHandle DepletionTimerHandle;
	class APlayerCharacter* PlayerCharacter;


	void SetIntensity();

	void FlickerLight();
	void StopFlickerLight();
};
