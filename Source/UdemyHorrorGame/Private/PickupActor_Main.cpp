// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupActor_Main.h"
#include "Components/SphereComponent.h"
#include "PlayerCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "InventoryComponent.h"
#include "Components/WidgetComponent.h"
#include "PickupPromptWidget.h"
#include "Components/Image.h"
#include "Camera/CameraComponent.h"
#include <EnhancedInputSubsystems.h>

// Sets default values
APickupActor_Main::APickupActor_Main()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	Sphere->SetupAttachment(StaticMesh);
	Sphere->SetGenerateOverlapEvents(true);
	Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	PromptWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Pickup Prompt Widget"));
	PromptWidgetComponent->SetupAttachment(StaticMesh);
	PromptWidgetComponent->SetWidgetClass(LoadClass<UUserWidget>(NULL, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/PickupActors/WBP_PickupPromptWidget.WBP_PickupPromptWidget_C'")));

	EPromptImage = LoadObject<UTexture2D>(NULL, TEXT("/Script/Engine.Texture2D'/Game/Textures/PickupItems/E_Prompt.E_Prompt'"));
	LMBPromptImage = LoadObject<UTexture2D>(NULL, TEXT("/Script/Engine.Texture2D'/Game/Textures/PickupItems/LMB_Prompt.LMB_Prompt'"));
	ArrowPromptImage = LoadObject<UTexture2D>(NULL, TEXT("/Script/Engine.Texture2D'/Game/Textures/PickupItems/Arrow_Prompt.Arrow_Prompt'"));
}

// Called when the game starts or when spawned
void APickupActor_Main::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}

void APickupActor_Main::Initialize()
{
	StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	PickupPromptWidget = Cast<UPickupPromptWidget>(PromptWidgetComponent->GetWidget());
	PlayerContr = UGameplayStatics::GetPlayerController(this, 0);

	Sphere->SetSphereRadius(ArrowPromptLength);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &APickupActor_Main::SphereOnBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &APickupActor_Main::SphereOnEndOverlap);
	Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	PromptWidgetComponent->SetVisibility(false);
	PromptWidgetComponent->SetWorldScale3D(FVector(0.03, 0.03, 0.03));
}

// Called every frame
void APickupActor_Main::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PromptWidgetComponent->SetWorldLocation(FVector(StaticMesh->GetComponentLocation()) + FVector(0, 0, WidgetDistanceAboveMesh));

	if (PlayerCharacter == nullptr) return;

	if (bIsPlayerOverlap)
	{
		FHitResult HitResult;
		FVector Start = PlayerCharacter->GetCameraComponent()->GetComponentLocation();
		FVector End = GetActorLocation();
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(PlayerCharacter);
		bool bWasHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, Params);
		AActor* HitActor = HitResult.GetActor();
		if (HitActor == this)
		{
			PromptWidgetComponent->SetVisibility(true);
		}
		else
		{
			PromptWidgetComponent->SetVisibility(false);
			PlayerCharacter->RemovePickupItem(this);
			return;
		}
		if ((PlayerCharacter->GetActorLocation() - GetActorLocation()).Size() <= PickupPromptLength)
		{
			TogglePrompt(true);
			PlayerCharacter->AddPickupItem(this);
		}
		else
		{
			TogglePrompt(false);
		}
		FRotator NewRotation((PlayerCharacter->GetActorLocation() - GetActorLocation()).Rotation());
		PromptWidgetComponent->SetWorldRotation(NewRotation);
	}
}


bool APickupActor_Main::ExecutePickup()
{
	if (PlayerCharacter != nullptr)
	{
		int Remainder = 0;
		if (PlayerCharacter->GetInventoryComponent()->AddItem(Item, Amount, &Remainder))
		{
			if (Remainder == 0)
			{
				Destroy();
				PlayerCharacter->RemovePickupItem(this);
			}
			return true;
		}
	}
	return false;
}

bool APickupActor_Main::Pickup_Implementation()
{
	return ExecutePickup();
}

void APickupActor_Main::TogglePrompt(bool bCanPickup)
{
	if (bCanPickup)
	{
		PickupPromptWidget->PromptImage->SetBrushFromTexture(LMBPromptImage);
	}
	else
	{
		PickupPromptWidget->PromptImage->SetBrushFromTexture(ArrowPromptImage);
	}
}

void APickupActor_Main::SphereOnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<APlayerCharacter>())
	{
		PromptWidgetComponent->SetVisibility(true);
		bIsPlayerOverlap = true;
	}
}

void APickupActor_Main::SphereOnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA<APlayerCharacter>())
	{
		PromptWidgetComponent->SetVisibility(false);
		bIsPlayerOverlap = false;
	}
}