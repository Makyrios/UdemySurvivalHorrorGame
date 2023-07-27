// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionActor_OutwardsDoor.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/Character.h"

AInteractionActor_OutwardsDoor::AInteractionActor_OutwardsDoor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AInteractionActor_OutwardsDoor::BeginPlay()
{
	Super::BeginPlay();


}

void AInteractionActor_OutwardsDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AInteractionActor_OutwardsDoor::OpenDoor(float Value)
{
	if (DotProduct < 0)
	{
		Value = -Value;
	}
	FRotator NewRotation(0, Value, 0);
	DoorStaticMesh->SetRelativeRotation(NewRotation);
}

void AInteractionActor_OutwardsDoor::Interact()
{
	if (bIsOpened)
	{
		TimelineComponent->Reverse();
		bIsOpened = false;
	}
	else
	{
		FVector DoorDirection = GetActorForwardVector();
		FVector PlayerDirection = UGameplayStatics::GetPlayerCharacter(this, 0)->GetActorForwardVector();
		DotProduct = FVector::DotProduct(DoorDirection, PlayerDirection);

		TimelineComponent->Play();
		bIsOpened = true;
	}
}
