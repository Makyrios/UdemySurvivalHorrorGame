// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "InventoryItem_Main.h"
#include "PickupActor_Main.generated.h"

UCLASS()
class UDEMYHORRORGAME_API APickupActor_Main : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupActor_Main();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere)
	class USphereComponent* Sphere;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Interact() override;

private:
	class APlayerCharacter* PlayerCharacter;
	
	UPROPERTY(EditAnywhere, Category = Item, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AInventoryItem_Main> Item;

	UPROPERTY(EditAnywhere, Category = Item, meta = (AllowPrivateAccess = "true"))
	int Amount;
};
