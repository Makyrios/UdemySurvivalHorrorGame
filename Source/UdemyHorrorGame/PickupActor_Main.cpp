// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupActor_Main.h"
#include "Components/SphereComponent.h"
#include "PlayerCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "InventoryComponent.h"

// Sets default values
APickupActor_Main::APickupActor_Main()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	RootComponent = StaticMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	Sphere->SetupAttachment(StaticMesh);
	//Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
}

// Called when the game starts or when spawned
void APickupActor_Main::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
}

// Called every frame
void APickupActor_Main::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupActor_Main::Interact()
{
	if (PlayerCharacter != nullptr)
	{
		int Remainder = 0;
		PlayerCharacter->GetInventoryComponent()->AddItem(Item, Amount, &Remainder);
		UE_LOG(LogTemp, Display, TEXT("Remainder: %d"), Remainder);
		if (Remainder == 0)
		{
			Destroy();
		}
	}
}

