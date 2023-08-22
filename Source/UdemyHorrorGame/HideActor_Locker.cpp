

// Fill out your copyright notice in the Description page of Project Settings.


#include "HideActor_Locker.h"
#include "PlayerCharacter.h"

AHideActor_Locker::AHideActor_Locker()
{
	PrimaryActorTick.bCanEverTick = true;

	DoorStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Static Mesh"));
	DoorStaticMesh->SetupAttachment(StaticMesh);
	DoorStaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);

	DoorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Door Timeline Component"));
}

void AHideActor_Locker::BeginPlay()
{
	Super::BeginPlay();

	if (DoorCurveFloat != nullptr)
	{
		DoorTimelineFloat.BindDynamic(this, &AHideActor_Locker::OpenDoor);
		DoorTimeline->AddInterpFloat(DoorCurveFloat, DoorTimelineFloat);
	}

	OnFinishedMoving.AddUObject(this, &AHideActor_Locker::OnFinishedMove_Event);
}


bool AHideActor_Locker::Interact()
{
	if (!bCanInteract) return false;
	PlayerCharacter->bCanOpenInventory = false;
	bCanInteract = false;
	DoorTimeline->Play();
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() { Super::Interact(); }, DoorTimeline->GetPlayRate() - 0.5f, false);
	return true;
}

void AHideActor_Locker::OpenDoor(float Value)
{
	UE_LOG(LogTemp, Display, TEXT("Value: %f"), Value);
	DoorStaticMesh->SetRelativeRotation(FRotator(0, Value, 0));
}


void AHideActor_Locker::OnFinishedMove_Event()
{
	DoorTimeline->Reverse();
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() 
		{
			bCanInteract = true;
		}, 
		DoorTimeline->GetPlayRate() - 0.5f, false);
}


