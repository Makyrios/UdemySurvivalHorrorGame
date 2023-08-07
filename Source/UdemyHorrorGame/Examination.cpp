// Fill out your copyright notice in the Description page of Project Settings.


#include "Examination.h"

// Sets default values
AExamination::AExamination()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));
}

// Called when the game starts or when spawned
void AExamination::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExamination::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

