// Fill out your copyright notice in the Description page of Project Settings.


#include "FlickeringSpotLight.h"
#include "Components/SpotLightComponent.h"

// Sets default values
AFlickeringSpotLight::AFlickeringSpotLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	PointLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Point Light"));
}

// Called when the game starts or when spawned
void AFlickeringSpotLight::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AFlickeringSpotLight::FlickerLight, FMath::RandRange(MinTime, MaxTime), true);
}

// Called every frame
void AFlickeringSpotLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFlickeringSpotLight::FlickerLight()
{
	PointLight->SetIntensity(FMath::RandRange(MinFlickerIntensity, MaxFlickerIntensity));
}

