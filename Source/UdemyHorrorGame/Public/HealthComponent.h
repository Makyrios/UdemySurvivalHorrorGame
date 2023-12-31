// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerDie, AActor* DamageCauser);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UDEMYHORRORGAME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Health)
	float MaxHealth = 100;
	UPROPERTY(EditAnywhere, Category = Health)
	float MinHealth = 0;

	UPROPERTY(EditAnywhere, Category = Health, meta = (ClampMin = "0", ClampMax = "1"))
	float BloodScreenActivationPercent = 0.7;

	FOnPlayerDie OnPlayerDieEvent;

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	void ChangeHealth(float Amount, AActor* DamageCauser = nullptr);

	inline float GetCurrentHealth() const { return CurrentHealth; }

	void UpdateBloodScreen();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	class UMaterialParameterCollection* BloodScreenMaterialCollection;

	float CurrentHealth = MaxHealth;
};
