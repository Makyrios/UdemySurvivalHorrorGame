// Fill out your copyright notice in the Description page of Project Settings.


#include "HideActor.h"
#include "Components/ArrowComponent.h"
#include <Kismet/GameplayStatics.h>
#include "PlayerCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AHideActor::AHideActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	RootComponent = StaticMesh;

	PlayerPosition = CreateDefaultSubobject<UArrowComponent>(TEXT("Player Position Arrow"));
	PlayerPosition->SetupAttachment(RootComponent);
	ExitPosition = CreateDefaultSubobject<UArrowComponent>(TEXT("Exit Position Arrow"));
	ExitPosition->SetupAttachment(RootComponent);


}

bool AHideActor::Interact()
{
	if (!PlayerCharacter->bIsHiding)
	{
		PlayerCharacter->SetActorEnableCollision(false);
		GetWorld()->GetTimerManager().SetTimer(MoveCharTimerHandle, this, &AHideActor::MoveCharacter, UGameplayStatics::GetWorldDeltaSeconds(this), true);
		PlayerCharacter->GetCharacterMovement()->DisableMovement();
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(MoveCharTimerHandle, this, &AHideActor::MoveCharacter, UGameplayStatics::GetWorldDeltaSeconds(this), true);
		PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
	return true;
}

// Called when the game starts or when spawned
void AHideActor::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
}

void AHideActor::MoveCharacter()
{
	if (!PlayerCharacter->bIsHiding)
	{
		FVector CurrentMove = FMath::VInterpConstantTo(PlayerCharacter->GetActorLocation(), PlayerPosition->GetComponentLocation(),
			UGameplayStatics::GetWorldDeltaSeconds(this), InterpolationSpeed);

		PlayerCharacter->SetActorLocation(CurrentMove);

		if (PlayerCharacter->GetActorLocation() == PlayerPosition->GetComponentLocation())
		{
			FinishedMoving();
			PlayerCharacter->bCanOpenInventory = false;
		}
	}
	else
	{
		FVector CurrentMove = FMath::VInterpConstantTo(PlayerCharacter->GetActorLocation(), ExitPosition->GetComponentLocation(),
			UGameplayStatics::GetWorldDeltaSeconds(this), InterpolationSpeed);

		PlayerCharacter->SetActorLocation(CurrentMove);

		if (PlayerCharacter->GetActorLocation() == ExitPosition->GetComponentLocation())
		{
			FinishedMoving();
			PlayerCharacter->SetActorEnableCollision(true);
			PlayerCharacter->bCanOpenInventory = true;
		}
	}
}

void AHideActor::FinishedMoving()
{
	GetWorld()->GetTimerManager().ClearTimer(MoveCharTimerHandle);
	PlayerCharacter->bIsHiding = !PlayerCharacter->bIsHiding;
	OnFinishedMoving.Broadcast();
}

// Called every frame
void AHideActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

