// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupActor_Main.h"
#include "Note_Main.generated.h"

UCLASS()
class UDEMYHORRORGAME_API ANote_Main : public APickupActor_Main
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, Category = Components)
	class UTextRenderComponent* FrontTextRenderComponent;
	UPROPERTY(VisibleAnywhere, Category = Components)
	class UTextRenderComponent* BackTextRenderComponent;

public:
	UPROPERTY(EditAnywhere, Category = Note)
	FText FrontNoteText;
	UPROPERTY(EditAnywhere, Category = Note)
	FText BackNoteText;

	UPROPERTY(EditAnywhere, Category = Examination)
	float ExaminationMeshOffset = 40;
	UPROPERTY(EditAnywhere, Category = Examination)
	FRotator ExaminationMeshRotation;

	// Sets default values for this actor's properties
	ANote_Main();

	void Pickup() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
