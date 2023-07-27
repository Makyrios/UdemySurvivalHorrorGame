// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grabbable.h"
#include "GrabActor.generated.h"

UCLASS()
class UDEMYHORRORGAME_API AGrabActor : public AActor, public IGrabbable
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;
	
public:	
	// Sets default values for this actor's properties
	AGrabActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bIsGrabbed = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Grab() override;
	virtual void Release() override;
};
