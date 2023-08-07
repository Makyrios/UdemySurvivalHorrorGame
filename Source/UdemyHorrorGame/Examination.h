// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Examination.generated.h"

UCLASS()
class UDEMYHORRORGAME_API AExamination : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = Components)
	class UStaticMeshComponent* ItemMesh;
	
public:	
	// Sets default values for this actor's properties
	AExamination();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	inline UStaticMeshComponent* GetItemMesh() const { return ItemMesh; }

private:

};
