// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabActor_PhysicsDoor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "PlayerCharacter.h"
#include <Kismet/GameplayStatics.h>

AGrabActor_PhysicsDoor::AGrabActor_PhysicsDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);

	DoorStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Static mesh"));
	DoorStaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);
	DoorStaticMesh->AttachToComponent(StaticMesh, FAttachmentTransformRules::KeepRelativeTransform);
	DoorStaticMesh->SetSimulatePhysics(true);

	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Physics Constraint"));
	PhysicsConstraint->SetupAttachment(DoorStaticMesh);
}

void AGrabActor_PhysicsDoor::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
}

void AGrabActor_PhysicsDoor::Tick(float DeltaTime)
{
	if (bIsGrabbed)
	{
		FVector2D LookAxis = PlayerCharacter->GetLookAxis();
		FVector Force(LookAxis.Y * ForceMultiplier * -1, LookAxis.X * ForceMultiplier, 0);
		MoveDoor(Force);
	}
}

void AGrabActor_PhysicsDoor::Grab()
{
	Super::Grab();

}

void AGrabActor_PhysicsDoor::Release()
{
	Super::Release();

}

void AGrabActor_PhysicsDoor::MoveDoor(FVector Force)
{
	FVector DoorDirection = GetActorForwardVector();
	FVector PlayerDirection = PlayerCharacter->GetActorForwardVector();
	float DotProduct = FVector::DotProduct(DoorDirection, PlayerDirection);
	if (DotProduct < 0)
	{
		Force = -Force;
	}
	DoorStaticMesh->AddForce(Force);
}
