// Fill out your copyright notice in the Description page of Project Settings.


#include "SecurityCameraController.h"
#include <Kismet/GameplayStatics.h>
#include "PlayerCharacter.h"
#include <EnhancedInputSubsystems.h>
#include "SecurityCamera.h"

// Sets default values
ASecurityCameraController::ASecurityCameraController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Camera Controller Mesh"));
	SetRootComponent(StaticMesh);
	StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
}

bool ASecurityCameraController::Interact()
{
	if (!bIsViewingCameras)
	{
		EnterCameraController();
	}
	else
	{
		ExitCameraController();
	}
	return true;
}

// Called when the game starts or when spawned
void ASecurityCameraController::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerContr = UGameplayStatics::GetPlayerController(this, 0);
	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	// Add all camera actors to array
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(this, SecurityCameraClass, FoundActors);
	for (auto& Actor : FoundActors)
	{
		SecurityCameras.Add(Cast<ASecurityCamera>(Actor));
	}
	// Set input
	if (PlayerCharacter != nullptr)
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerCharacter->InputComponent))
		{
			EnhancedInputComponent->BindAction(ShowNextCameraAction, ETriggerEvent::Completed, this, &ASecurityCameraController::ShowNextCamera);
			EnhancedInputComponent->BindAction(ShowPrevCameraAction, ETriggerEvent::Completed, this, &ASecurityCameraController::ShowPrevCamera);
		}

		PlayerCharacter->PressedReturnEvent.AddUObject(this, &ASecurityCameraController::ExitCameraController);
	}
}

// Called every frame
void ASecurityCameraController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASecurityCameraController::EnterCameraController()
{
	if (!bIsViewingCameras)
	{
		bIsViewingCameras = true;
		DisableInput(PlayerContr);
		PlayerContr->SetIgnoreLookInput(true);
		PlayerCharacter->ToggleHUD();

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerContr->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CameraMappingContext, 1);
		}

		ViewThroughCamera();
	}
}

void ASecurityCameraController::ExitCameraController()
{
	if (bIsViewingCameras)
	{
		bIsViewingCameras = false;
		EnableInput(PlayerContr);
		PlayerContr->ResetIgnoreLookInput();
		PlayerCharacter->ToggleHUD();

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerContr->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(CameraMappingContext);
		}

		PlayerContr->SetViewTargetWithBlend(PlayerCharacter);
	}
}

void ASecurityCameraController::ShowNextCamera()
{
	if (++CurrentCameraIndex >= SecurityCameras.Num())
	{
		CurrentCameraIndex = 0;
	}
	ViewThroughCamera();
}

void ASecurityCameraController::ShowPrevCamera()
{
	if (--CurrentCameraIndex < 0)
	{
		CurrentCameraIndex = SecurityCameras.Num() - 1;
	}
	ViewThroughCamera();
}

void ASecurityCameraController::ViewThroughCamera()
{
	if (SecurityCameras.IsValidIndex(CurrentCameraIndex))
	{
		PlayerContr->SetViewTargetWithBlend(SecurityCameras[CurrentCameraIndex]);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Current camera index is not valid"));
	}
}

