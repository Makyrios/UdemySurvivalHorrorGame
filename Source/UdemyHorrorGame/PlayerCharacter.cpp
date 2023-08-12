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
#include "PickupActor_Main.h"
#include "FlashlightComponent.h"
#include "HealthComponent.h"
#include "MainHUDWidget.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComponent->SetupAttachment(RootComponent);
	CameraComponent->bUsePawnControlRotation = true;

	SpotlightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("Spotlight Component"));
	SpotlightComponent->SetupAttachment(SpringArmComponent);

	MoveComponent = CreateDefaultSubobject<UMoveComponent>(TEXT("Move Component"));
	CrouchTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("Crouch Timeline Component"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));
	FlashlightComponent = CreateDefaultSubobject<UFlashlightComponent>(TEXT("Flashlight Component"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
}

void APlayerCharacter::AddPickupItem(APickupActor_Main* PickUpItem)
{
	CurrentPickupItems.AddUnique(PickUpItem);
	CheckPickupContext();
}

void APlayerCharacter::RemovePickupItem(APickupActor_Main* PickUpItem)
{
	CurrentPickupItems.Remove(PickUpItem);
	CheckPickupContext();
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
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{

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

		//Pickup Item
		EnhancedInputComponent->BindAction(PickupItemAction, ETriggerEvent::Completed, this, &APlayerCharacter::PickupItem);

		EnhancedInputComponent->BindAction(ReturnAction, ETriggerEvent::Started, this, &APlayerCharacter::Return);
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
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
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
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::Interact()
{
	AActor* HitActor = LineTrace(TraceLength);
	if (IInteractable* InteractActor = Cast<IInteractable>(HitActor))
	{
		InteractActor->Interact();
	}
	// After interact update pickup context
	if (CurrentPickupItems.Num() > 0)
	{
		CheckPickupContext();
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
	FlashlightComponent->ToggleFlashlight();
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
	UInventoryMenuWidget* InventoryMenu = InventoryComponent->GetInventoryMenuWidget();
	if (!bIsInventoryOpen)
	{
		bIsInventoryOpen = true;
		GetCharacterMovement()->DisableMovement();
		PlayerController->SetIgnoreLookInput(true);
		PlayerController->bShowMouseCursor = true;
		InventoryMenu->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(InventoryMenu->GetCachedWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(true);
		if (ExaminationWidget->IsVisible())
		{
			ExaminationWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		PlayerController->SetInputMode(InputMode);
	}
	else
	{
		bIsInventoryOpen = false;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		PlayerController->ResetIgnoreLookInput();
		PlayerController->bShowMouseCursor = false;
		InventoryMenu->SetVisibility(ESlateVisibility::Collapsed);
		InventoryMenu->HideItemDetails();
		InventoryMenu->CloseDropdownMenu();
		PlayerController->SetInputMode(FInputModeGameOnly());
	}

}

void APlayerCharacter::PickupItem()
{
	if (CurrentPickupItems.Num() > 0)
	{
		// If player looking at an interactable actor, then interacting with it will be prioritized
		// Otherwise pickup actor
		if (Cast<IInteractable>(LineTrace(TraceLength)))
		{
			//if (APlayerController* PlayerContr = Cast<APlayerController>(Controller))
			//{
			//	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerContr->GetLocalPlayer()))
			//	{
			//		Subsystem->RemoveMappingContext(PickupItemMappingContext);
			//		//Subsystem->AddMappingContext(DefaultMappingContext, 0);
			//	}
			//}
			Interact();
		}
		else
		{
			CurrentPickupItems.Top()->Pickup();
		}
	}
}

void APlayerCharacter::Return()
{
	PressedReturnEvent.Broadcast();
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

void APlayerCharacter::CheckPickupContext()
{
	if (CurrentPickupItems.Num() > 0)
	{
		if (APlayerController* PlayerContr = Cast<APlayerController>(Controller))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerContr->GetLocalPlayer()))
			{
				//Subsystem->RemoveMappingContext(DefaultMappingContext);
				Subsystem->AddMappingContext(PickupItemMappingContext, 1);
			}
		}
	}
	else
	{
		if (APlayerController* PlayerContr = Cast<APlayerController>(Controller))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerContr->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(PickupItemMappingContext);
				//Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}



void APlayerCharacter::Initialize()
{
	PlayerController = UGameplayStatics::GetPlayerController(this, 0);


	//HUD
	if (MainHUDClass)
	{
		UMainHUDWidget* MainHUDWidget = CreateWidget<UMainHUDWidget>(Cast<AHG_PlayerController>(GetController()), MainHUDClass);
		MainHUDWidget->InitializeWidget(this);
		MainHUDWidget->AddToViewport();
	}

	//Mapping context
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

	//Flashlight component
	FlashlightComponent->Initialize(this);
}