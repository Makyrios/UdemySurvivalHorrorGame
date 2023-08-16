// Fill out your copyright notice in the Description page of Project Settings.


#include "NoteExamination.h"
#include "Components/TextRenderComponent.h"


// Sets default values
ANoteExamination::ANoteExamination()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Note Item Mesh"));

	FrontTextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Front Note Text"));
	FrontTextRenderComponent->SetupAttachment(ItemMesh);
	BackTextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Back Note Text"));
	BackTextRenderComponent->SetupAttachment(ItemMesh);
}

void ANoteExamination::SetFrontText(FText& Text)
{
	FrontTextRenderComponent->SetText(Text);
}

void ANoteExamination::SetBackText(FText& Text)
{
	BackTextRenderComponent->SetText(Text);
}

// Called when the game starts or when spawned
void ANoteExamination::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANoteExamination::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

