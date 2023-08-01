// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interactable.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/UserWidget.h"
#include "HG_PlayerController.h"
#include "Grabbable.h"
#include "Components/SpotLightComponent.h"
#include "MoveComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/GameplayStatics.h>
#include "InventoryComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComponent->SetupAttachment(RootComponent);

	SpotlightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("Spotlight Component"));
	SpotlightComponent->SetupAttachment(SpringArmComponent);

	MoveComponent = CreateDefaultSubobject<UMoveComponent>(TEXT("Move Component"));

	CrouchTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("Crouch Timeline Component"));

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	MoveComponent->Initialize(this);
	Initialize();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Look
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		//Move
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		//Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &APlayerCharacter::Interact);

		//Grab
		EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Started, this, &APlayerCharacter::StartGrab);
		EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopGrab);

		//Toggle Flashlight
		EnhancedInputComponent->BindAction(ToggleFlashlightAction, ETriggerEvent::Started, this, &APlayerCharacter::ToggleFlashlight);

		//Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &APlayerCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopSprint);

		//Crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &APlayerCharacter::StartCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopCrouch);

		//Toggle Inventory
		EnhancedInputComponent->BindAction(ToggleInventoryAction, ETriggerEvent::Completed, this, &APlayerCharacter::ToggleInventory);
	}
}

void APlayerCharacter::ShortenPlayerCapsule()
{
	CrouchTimelineComponent->Play();
}

void APlayerCharacter::LengthenPlayerCapsule()
{
	CrouchTimelineComponent->Reverse();
}


void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
	HeadBob();
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	LookAxis = LookAxisVector;

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(-LookAxisVector.Y);
	}
}

void APlayerCharacter::Interact()
{
	AActor* HitActor = LineTrace(TraceLength);
	if (IInteractable* InteractActor = Cast<IInteractable>(HitActor))
	{
		InteractActor->Interact();
	}
}

void APlayerCharacter::StartGrab()
{
	AActor* HitActor = LineTrace(TraceLength);
	if (IGrabbable* InteractActor = Cast<IGrabbable>(HitActor))
	{
		GrabbedActor = InteractActor;
		InteractActor->Grab();
	}
}

void APlayerCharacter::StopGrab()
{
	if (GrabbedActor != nullptr)
	{
		GrabbedActor->Release();
		GrabbedActor = nullptr;
	}
}

void APlayerCharacter::ToggleFlashlight()
{
	if (!SpotlightComponent->IsVisible())
	{
		SpotlightComponent->SetVisibility(true);
	}
	else
	{
		SpotlightComponent->SetVisibility(false);
	}
}

void APlayerCharacter::StartSprint()
{
	MoveComponent->StartSprint();
}

void APlayerCharacter::StopSprint()
{
	MoveComponent->StopSprint();
}

void APlayerCharacter::StartCrouch()
{
	MoveComponent->StartCrouch();
}

void APlayerCharacter::StopCrouch()
{
	MoveComponent->StopCrouch();
}

void APlayerCharacter::ToggleInventory()
{
	if (!bIsPaused)
	{
		bIsPaused = true;
		GetCharacterMovement()->DisableMovement();
		PlayerController->SetIgnoreLookInput(true);
		PlayerController->bShowMouseCursor = true;
		InventoryMenuWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(InventoryMenuWidget->GetCachedWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(true);
		PlayerController->SetInputMode(InputMode);
	}
	else
	{
		bIsPaused = false;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		PlayerController->SetIgnoreLookInput(false);
		PlayerController->bShowMouseCursor = false;
		InventoryMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
		PlayerController->SetInputMode(FInputModeGameOnly());
	}
	
}

AActor* APlayerCharacter::LineTrace(float Length)
{
	FHitResult HitResult;
	FVector Start = CameraComponent->GetComponentLocation();
	FVector End = Start + CameraComponent->GetForwardVector() * Length;
	FCollisionQueryParams Params;
	Params.TraceTag = "Trace";
	GetWorld()->DebugDrawTraceTag = "Trace";
	bool bWasHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_GameTraceChannel1, Params);

	return HitResult.GetActor();
}

void APlayerCharacter::SetCapsuleHalfHeight(float Amount)
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(Amount);
}

void APlayerCharacter::HeadBob()
{
	if (!WalkHeadBob || !RunHeadBob)
	{
		UE_LOG(LogTemp, Warning, TEXT("Head bobs are null"));
		return;
	}
	if (GetCharacterMovement()->Velocity.Size() >= MoveComponent->GetWalkSpeed() && !GetCharacterMovement()->IsFalling())
	{
		if (GetCharacterMovement()->Velocity.Size() >= MoveComponent->GetSprintSpeed() && !GetCharacterMovement()->IsFalling())
		{
			TRange<float> Range1 { 0.f, MoveComponent->GetSprintSpeed() };
			TRange<float> Range2 {0, 1};
			float BobScale = FMath::GetMappedRangeValueClamped(Range1, Range2, (float)GetCharacterMovement()->Velocity.Size());
			PlayerController->ClientStartCameraShake(RunHeadBob, BobScale);
		}
		else
		{
			TRange<float> Range1 { 0.f, MoveComponent->GetWalkSpeed() };
			TRange<float> Range2 {0, 1};
			float BobScale = FMath::GetMappedRangeValueClamped(Range1, Range2, (float)GetCharacterMovement()->Velocity.Size());
			PlayerController->ClientStartCameraShake(WalkHeadBob);
		}
	}
}

void APlayerCharacter::Initialize()
{
	//HUD
	if (MainHUDClass)
	{
		UUserWidget* MainHUDWidget = CreateWidget<UUserWidget>(Cast<AHG_PlayerController>(GetController()), MainHUDClass);
		MainHUDWidget->AddToViewport();
	}
	if (InventoryMenuClass)
	{
		UInventoryMenuWidget* MenuWidget = CreateWidget<UInventoryMenuWidget>(Cast<AHG_PlayerController>(GetController()), InventoryMenuClass);
		MenuWidget->AddToViewport();
		MenuWidget->SetVisibility(ESlateVisibility::Collapsed);
		InventoryMenuWidget = MenuWidget;
	}

	if (APlayerController* PlayerContr = Cast<APlayerController>(Controller))
	{
		//Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerContr->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	

	//Crouch timeline
	if (CrouchCurveFloat != nullptr)
	{
		CrouchCurveFloat->FloatCurve.Keys[0].Value = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		float TimeToCrouch = (GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - MoveComponent->GetCrouchHalfHeight()) / MoveComponent->GetCrouchingSpeed();
		CrouchCurveFloat->FloatCurve.Keys[1].Time = TimeToCrouch;
		CrouchCurveFloat->FloatCurve.Keys[1].Value = MoveComponent->GetCrouchHalfHeight();

		CrouchTimelineFloat.BindDynamic(this, &APlayerCharacter::SetCapsuleHalfHeight);
		CrouchTimelineComponent->AddInterpFloat(CrouchCurveFloat, CrouchTimelineFloat);
	}

	PlayerController = UGameplayStatics::GetPlayerController(this, 0);


}
