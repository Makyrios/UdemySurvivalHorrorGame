// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrabActor.h"
#include "GrabActor_PhysicsDoor.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYHORRORGAME_API AGrabActor_PhysicsDoor : public AGrabActor
{
	GENERATED_BODY()

public:
	AGrabActor_PhysicsDoor();

	UPROPERTY(EditAnywhere, Category = Physics)
	float ForceMultiplier = 5000;

protected:
	void BeginPlay() override;

	void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* DoorStaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UPhysicsConstraintComponent* PhysicsConstraint; 
	
public:
	void Grab() override;
	void Release() override;

	void MoveDoor(FVector Force);

private:
	class APlayerCharacter* PlayerCharacter;
};
