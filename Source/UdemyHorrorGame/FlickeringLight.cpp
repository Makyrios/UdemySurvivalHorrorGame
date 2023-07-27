// Fill out your copyright notice in the Description page of Project Settings.


#include "FlickeringLight.h"
#include "Components/PointLightComponent.h"

// Sets default values
AFlickeringLight::AFlickeringLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Point Light"));
}

// Called when the game starts or when spawned
void AFlickeringLight::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AFlickeringLight::FlickerLight, FMath::RandRange(MinTime, MaxTime), true);
}

// Called every frame
void AFlickeringLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFlickeringLight::FlickerLight()
{
	PointLight->SetIntensity(FMath::RandRange(MinFlickerIntensity, MaxFlickerIntensity));
}

