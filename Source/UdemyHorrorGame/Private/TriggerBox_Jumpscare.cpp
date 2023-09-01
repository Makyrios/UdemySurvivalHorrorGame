// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerBox_Jumpscare.h"
#include "Sound/SoundCue.h"
#include <Kismet/GameplayStatics.h>
#include "PlayerCharacter.h"

void ATriggerBox_Jumpscare::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Other->IsA<APlayerCharacter>())
	{
		UGameplayStatics::PlaySound2D(this, JumpscareCue);
		Destroy();
	}
}
