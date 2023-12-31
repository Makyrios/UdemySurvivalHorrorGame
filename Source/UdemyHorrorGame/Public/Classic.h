// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Classic.generated.h"

UCLASS()
class UDEMYHORRORGAME_API AClassic : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Animations)
	class UAnimMontage* CatchAnimMontage;

public:
	UPROPERTY(EditAnywhere, Category = AI)
	class UBehaviorTree* BotBehavior;

	// Sets default values for this character's properties
	AClassic();


	void CatchTarget(AActor* Target);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	inline TArray<class AAITargetPoint*> GetRoomTargetPoints() const { return RoomTargetPoints; }

private:
	UPROPERTY(EditInstanceOnly, Category = Patrol)
	TArray<class AAITargetPoint*> RoomTargetPoints;

};
