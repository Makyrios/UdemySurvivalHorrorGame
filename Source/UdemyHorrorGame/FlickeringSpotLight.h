// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlickeringSpotLight.generated.h"

UCLASS()
class UDEMYHORRORGAME_API AFlickeringSpotLight : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class USpotLightComponent* PointLight;
	
public:	
	// Sets default values for this actor's properties
	AFlickeringSpotLight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = Light)
	float MinFlickerIntensity = 3500;

	UPROPERTY(EditAnywhere, Category = Light)
	float MaxFlickerIntensity = 5000;

	UPROPERTY(EditAnywhere, Category = Light)
	float MinTime = 0.1;

	UPROPERTY(EditAnywhere, Category = Light)
	float MaxTime = 0.3;


private:
	FTimerHandle TimerHandle;

	UFUNCTION()
	void FlickerLight();

};
