// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabActor.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AGrabActor::AGrabActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
}

// Called when the game starts or when spawned
void AGrabActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrabActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrabActor::Grab()
{
	bIsGrabbed = true;
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(this, 0);
	Character->GetCharacterMovement()->DisableMovement();
	AController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	Controller->SetIgnoreLookInput(true);
	EnableInput(Cast<APlayerController>(Controller));
}

void AGrabActor::Release()
{
	bIsGrabbed = false;
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(this, 0);
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	AController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	Controller->SetIgnoreLookInput(false);
	DisableInput(Cast<APlayerController>(Controller));
}

