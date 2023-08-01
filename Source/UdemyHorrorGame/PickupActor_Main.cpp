// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupActor_Main.h"

// Sets default values
APickupActor_Main::APickupActor_Main()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APickupActor_Main::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupActor_Main::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

