// Fill out your copyright notice in the Description page of Project Settings.


#include "FlickeringPointLight.h"
#include "Components/PointLightComponent.h"

// Sets default values
AFlickeringPointLight::AFlickeringPointLight()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Point Light"));
}

// Called when the game starts or when spawned
void AFlickeringPointLight::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AFlickeringPointLight::FlickerLight, FMath::RandRange(MinTime, MaxTime), true);
}

// Called every frame
void AFlickeringPointLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFlickeringPointLight::FlickerLight()
{
	PointLight->SetIntensity(FMath::RandRange(MinFlickerIntensity, MaxFlickerIntensity));
}

