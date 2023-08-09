// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveComponent.h"
#include <Kismet/GameplayStatics.h>
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UMoveComponent::UMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UMoveComponent::Initialize(APlayerCharacter* PlayerRef)
{
	PlayerCharacter = PlayerRef;
	SetMaxWalkSpeed(WalkSpeed);
}

void UMoveComponent::StartSprint()
{
	if (bIsCrouching) return;
	if (CurrentStamina > MinStamina)
	{
		SetMaxWalkSpeed(SprintSpeed);
	}
	GetWorld()->GetTimerManager().SetTimer(SprintTimerHandle, this, &UMoveComponent::SprintTimer, StaminaConsumptionRate, true);
}

void UMoveComponent::StopSprint()
{
	GetWorld()->GetTimerManager().ClearTimer(SprintTimerHandle);
	SetMaxWalkSpeed(WalkSpeed);
	GetWorld()->GetTimerManager().SetTimer(RegenerateTimerHandle, this, &UMoveComponent::RegenerateStamina, StaminaConsumptionRate, true, StaminaRegenerationDelay);
}

void UMoveComponent::StartCrouch()
{
	GetWorld()->GetTimerManager().ClearTimer(SprintTimerHandle);
	SetMaxWalkSpeed(CrouchSpeed);
	PlayerCharacter->ShortenPlayerCapsule();
	bIsCrouching = true;
}

void UMoveComponent::StopCrouch()
{
	SetMaxWalkSpeed(WalkSpeed);
	PlayerCharacter->LengthenPlayerCapsule();
	bIsCrouching = false;
}


// Called when the game starts
void UMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMoveComponent::SetMaxWalkSpeed(float Speed)
{
	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed;
}

float UMoveComponent::GetPlayerSpeed() const
{
	return PlayerCharacter->GetCharacterMovement()->Velocity.Size();
}

void UMoveComponent::SprintTimer()
{
	UE_LOG(LogTemp, Display, TEXT("Sprint %f"), CurrentStamina);
	// Player staying still
	if (GetPlayerSpeed() == 0) return;
	if (CurrentStamina > MinStamina)
	{
		CurrentStamina = FMath::Clamp(CurrentStamina - StaminaConsumption, MinStamina, MaxStamina);
	}
	else
	{
		StopSprint();
	}
}

void UMoveComponent::RegenerateStamina()
{
	UE_LOG(LogTemp, Display, TEXT("Sprint %f"), CurrentStamina);
	if (GetPlayerSpeed() == SprintSpeed || CurrentStamina == MaxStamina)
	{
		GetWorld()->GetTimerManager().ClearTimer(RegenerateTimerHandle);
		return;
	}
	CurrentStamina = FMath::Clamp(CurrentStamina + StaminaRegenerationDelay, MinStamina, MaxStamina);
}

