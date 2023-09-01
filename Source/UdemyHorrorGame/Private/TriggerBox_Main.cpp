
// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerBox_Main.h"
#include "Components/ShapeComponent.h"

void ATriggerBox_Main::BeginPlay()
{
	Super::BeginPlay();

	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATriggerBox_Main::OnBeginOverlap);
}

void ATriggerBox_Main::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}
