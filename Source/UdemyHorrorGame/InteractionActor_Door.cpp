

// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionActor_Door.h"

AInteractionActor_Door::AInteractionActor_Door()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DoorStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Static Mesh"));
	DoorStaticMesh->AttachToComponent(StaticMesh, FAttachmentTransformRules::KeepRelativeTransform);

	StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	DoorStaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);

	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("Door Timeline Component"));
}

void AInteractionActor_Door::BeginPlay()
{
	Super::BeginPlay();


	if (DoorCurveFloat != nullptr)
	{
		DoorTimelineFloat.BindDynamic(this, &AInteractionActor_Door::OpenDoor);
		TimelineComponent->AddInterpFloat(DoorCurveFloat, DoorTimelineFloat);
	}
}

void AInteractionActor_Door::Interact()
{
	Super::Interact();

	if (bIsOpened)
	{
		TimelineComponent->Reverse();
		bIsOpened = false;
	}
	else
	{
		TimelineComponent->Play();
		bIsOpened = true;
	}
}

void AInteractionActor_Door::OpenDoor(float Value)
{
	FRotator NewRotation(0, Value, 0);
	DoorStaticMesh->SetWorldRotation(NewRotation);
}
