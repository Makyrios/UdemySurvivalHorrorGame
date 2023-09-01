// Fill out your copyright notice in the Description page of Project Settings.


#include "AITargetPoint.h"

AAITargetPoint::AAITargetPoint()
{
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(RootComponent);
}
