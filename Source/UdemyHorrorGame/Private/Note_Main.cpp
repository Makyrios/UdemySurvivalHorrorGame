// Fill out your copyright notice in the Description page of Project Settings.


#include "Note_Main.h"
#include "Components/TextRenderComponent.h"
#include "Components/SphereComponent.h"
#include "PlayerCharacter.h"
#include "InventoryComponent.h"

// Sets default values
ANote_Main::ANote_Main()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FrontTextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Front Note Text"));
	FrontTextRenderComponent->SetupAttachment(StaticMesh);

	BackTextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Back Note Text"));
	BackTextRenderComponent->SetupAttachment(StaticMesh);
}

// Called when the game starts or when spawned
void ANote_Main::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANote_Main::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	FrontTextRenderComponent->SetText(FrontNoteText);
	BackTextRenderComponent->SetText(BackNoteText);
}

bool ANote_Main::ExecutePickup()
{
	if (PlayerCharacter != nullptr)
	{
		PlayerCharacter->GetInventoryComponent()->ExamineItem(this);
		return true;
	}
	return false;
}

// Called every frame
void ANote_Main::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

