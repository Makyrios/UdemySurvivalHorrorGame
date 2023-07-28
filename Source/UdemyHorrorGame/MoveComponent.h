// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoveComponent.generated.h"

class APlayerCharacter;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UDEMYHORRORGAME_API UMoveComponent : public UActorComponent
{
	GENERATED_BODY()


public:	
	// Sets default values for this component's properties
	UMoveComponent();

	void Initialize(APlayerCharacter* PlayerRef);

	void StartSprint();
	void StopSprint();

	void StartCrouch();
	void StopCrouch();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE float GetCrouchingSpeed() const { return CrouchingSpeed; }
	FORCEINLINE float GetCrouchHalfHeight() const { return CrouchHalfHeight; }


private:
	void SetMaxWalkSpeed(float Speed);
	float GetPlayerSpeed() const;

	UFUNCTION()
	void SprintTimer();

	UFUNCTION()
	void RegenerateStamina();

private:
	FTimerHandle SprintTimerHandle;
	FTimerHandle RegenerateTimerHandle;
	bool bIsCrouching = false;

	class APlayerCharacter* PlayerCharacter;
		
	UPROPERTY(EditAnywhere, Category = Movement)
	float WalkSpeed = 500;
	UPROPERTY(EditAnywhere, Category = Movement)
	float SprintSpeed = 800;
	UPROPERTY(EditAnywhere, Category = Movement)
	float CrouchSpeed = 300;

	UPROPERTY(EditAnywhere, Category = Movement)
	float StaminaConsumption = 1;
	UPROPERTY(EditAnywhere, Category = Movement)
	float StaminaRegenerationRate = 1.5;

	UPROPERTY(EditAnywhere, Category = Movement)
	float StaminaRegenerationDelay = 5;
	
	UPROPERTY(EditAnywhere, Category = Movement)
	float MaxStamina = 100;
	UPROPERTY(EditAnywhere, Category = Crouch)
	float CrouchingSpeed = 200;
	UPROPERTY(EditAnywhere, Category = Crouch)
	float CrouchHalfHeight = 44;

	float MinStamina = 0;
	float CurrentStamina = MaxStamina;
	
};
