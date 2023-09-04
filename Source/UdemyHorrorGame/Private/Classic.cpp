// Fill out your copyright notice in the Description page of Project Settings.


#include "Classic.h"
#include "PlayerCharacter.h"
#include <HG_PlayerController.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
AClassic::AClassic()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AClassic::CatchTarget(AActor* Target)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Target))
	{
		PlayerCharacter->DamagePlayer(100, this);
	}
	if (CatchAnimMontage != nullptr)
	{
		AHG_PlayerController* PlayerContr = Cast<AHG_PlayerController>(UGameplayStatics::GetPlayerController(this, 0));
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, PlayerContr]()
			{
				float Length = PlayAnimMontage(CatchAnimMontage);
				FTimerHandle TimerHandle2;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle2, [PlayerContr]() {
					PlayerContr->GameOver();
					}, Length, false);
			},
			0.7, false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Catch anim montage is null"));
	}

}

// Called when the game starts or when spawned
void AClassic::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AClassic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

