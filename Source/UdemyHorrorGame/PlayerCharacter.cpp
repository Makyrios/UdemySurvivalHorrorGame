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
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Sound/SoundCue.h"
#include "HG_GameStateBase.h"
#include "Components/Image.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComponent->SetupAttachment(RootComponent);
	CameraComponent->bUsePawnControlRotation = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArmComponent->SetupAttachment(CameraComponent);

	SpotlightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("Spotlight Component"));
	SpotlightComponent->SetupAttachment(SpringArmComponent);

	MoveComponent = CreateDefaultSubobject<UMoveComponent>(TEXT("Move Component"));
	CrouchTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("Crouch Timeline Component"));
	FootstepTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("Footstep Timeline Component"));
	LeaningTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("Leaning Timeline Component"));
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

void APlayerCharacter::ToggleHUD()
{
	if (!MainHUDWidget) return;

	if (MainHUDWidget->IsVisible())
	{
		MainHUDWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		MainHUDWidget->SetVisibility(ESlateVisibility::Visible);
	}
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

	UpdateDOF();
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

		//Return
		EnhancedInputComponent->BindAction(ReturnAction, ETriggerEvent::Started, this, &APlayerCharacter::Return);

		//Pickup Item Context
		EnhancedInputComponent->BindAction(PickupItemAction, ETriggerEvent::Completed, this, &APlayerCharacter::PickupItem);

		// LMB Action
		EnhancedInputComponent->BindAction(LMBAction, ETriggerEvent::Started, this, &APlayerCharacter::LMBPress);
		EnhancedInputComponent->BindAction(LMBAction, ETriggerEvent::Completed, this, &APlayerCharacter::LMBRelease);

		// Leaning Action
		EnhancedInputComponent->BindAction(LeanLeftAction, ETriggerEvent::Started, this, &APlayerCharacter::StartLeanLeft);
		EnhancedInputComponent->BindAction(LeanLeftAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopLeanLeft);
		EnhancedInputComponent->BindAction(LeanRightAction, ETriggerEvent::Started, this, &APlayerCharacter::StartLeanRight);
		EnhancedInputComponent->BindAction(LeanRightAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopLeanRight);

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
	TRange<float> R1 {MoveComponent->CrouchingSpeed, MoveComponent->SprintSpeed};
	TRange<float> R2 {0.7, 1.3};
	float MapValue = FMath::GetMappedRangeValueClamped(R1, R2, (float)GetCharacterMovement()->Velocity.Size());
	FootstepTimelineComponent->SetPlayRate(MapValue);
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
	if (!bCanOpenInventory) return;

	UInventoryMenuWidget* InventoryMenu = InventoryComponent->GetInventoryMenuWidget();
	if (!bIsInventoryOpen)
	{
		bIsInventoryOpen = true;
		GetCharacterMovement()->DisableMovement();
		PlayerController->SetIgnoreLookInput(true);
		PlayerController->bShowMouseCursor = true;
		InventoryMenu->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		MainHUDWidget->SetVisibility(ESlateVisibility::Hidden);
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
		MainHUDWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
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

void APlayerCharacter::LMBPress()
{
	LMBPressedEvent.Broadcast();
}

void APlayerCharacter::LMBRelease()
{
	LMBReleasedEvent.Broadcast();
}

void APlayerCharacter::StartLeanLeft()
{
	if (!bIsLeaningRight && !bIsLeaningLeft)
	{
		bIsLeaningLeft = true;
		LeaningTimelineComponent->Play();
	}
}

void APlayerCharacter::StopLeanLeft()
{
	if (bIsLeaningLeft)
	{
		bIsReversingLeaning = true;
		LeaningTimelineComponent->Reverse();
	}
}

void APlayerCharacter::StartLeanRight()
{
	if (!bIsLeaningLeft && !bIsLeaningRight)
	{
		bIsLeaningRight = true;
		LeaningTimelineComponent->Play();
	}
}

void APlayerCharacter::StopLeanRight()
{
	if (bIsLeaningRight)
	{
		bIsReversingLeaning = true;
		LeaningTimelineComponent->Reverse();
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

void APlayerCharacter::UpdateFootstep(float Amount)
{
	if (FootstepCurveFloat->FloatCurve.FindKey(Amount) != FKeyHandle::Invalid())
	{
		PlayFootstep();
	}
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

void APlayerCharacter::UpdateDOF()
{
	FHitResult HitResult;
	FVector Start = GetCameraComponent()->GetComponentLocation();
	FVector End = Start + GetCameraComponent()->GetForwardVector() * DOFLength;
	bool bWasHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility);
	if (bWasHit)
	{
		FVector Location = { HitResult.Location.X, HitResult.Location.Y, HitResult.Location.Z };
		float FocalDistance = (Location - Start).Size();
		GetCameraComponent()->PostProcessSettings.DepthOfFieldFocalDistance = FocalDistance;
		UE_LOG(LogTemp, Display, TEXT("Focal Distance: %f"), FocalDistance);
		GetCameraComponent()->PostProcessSettings.DepthOfFieldDepthBlurRadius = 350 / (FocalDistance);
		UE_LOG(LogTemp, Display, TEXT("%f"), GetCameraComponent()->PostProcessSettings.DepthOfFieldDepthBlurRadius);
	}
	else
	{
		GetCameraComponent()->PostProcessSettings.DepthOfFieldFocalDistance = 0;
		GetCameraComponent()->PostProcessSettings.DepthOfFieldDepthBlurRadius = 0;
	}
}

void APlayerCharacter::PlayFootstep()
{
	if (GetCharacterMovement()->Velocity.Size() > 10 && !GetCharacterMovement()->IsFalling())
	{
		FHitResult HitResult;
		FVector Start = GetActorLocation();
		FVector End = FVector(Start.X, Start.Y, Start.Z - 100);
		FCollisionQueryParams Params;
		Params.bReturnPhysicalMaterial = true;
		bool bWasHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, Params);
		if (bWasHit)
		{
			if (UPhysicalMaterial* PhysMaterial = HitResult.PhysMaterial.Get())
			{ 
				TRange<float> R1 {GetMoveComponent()->CrouchingSpeed, GetMoveComponent()->SprintSpeed };
				TRange<float> R2 { 0.5, 1.2 };
				float SoundMultiplier = FMath::GetMappedRangeValueClamped(R1, R2, (float)GetCharacterMovement()->Velocity.Size());
				if (PhysMaterial == TilePhysMaterial)
				{
					UGameplayStatics::PlaySoundAtLocation(this, TileFootstepsSoundCue, FVector(HitResult.Location.X, HitResult.Location.Y, HitResult.Location.Z), SoundMultiplier);
				}
				else if (PhysMaterial == GrassPhysMaterial)
				{
					UGameplayStatics::PlaySoundAtLocation(this, GrassFootstepsSoundCue, FVector(HitResult.Location.X, HitResult.Location.Y, HitResult.Location.Z), SoundMultiplier);
				}
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("Hitres: %s"), *HitResult.ToString());
			}
		}
	}
}

void APlayerCharacter::LeanCamera(float Amount)
{
	UE_LOG(LogTemp, Display, TEXT("%f"), Amount);
	if (bIsLeaningLeft)
	{
		TRange<float> CurveRange {0, 1};
		TRange<float> RollRange {0, -15};
		float RollValue = FMath::GetMappedRangeValueClamped(CurveRange, RollRange, Amount);
		FRotator CameraRot = Controller->GetControlRotation();
		CameraRot.Roll = RollValue;
		Controller->SetControlRotation(CameraRot);
		TRange<float> OffsetRange {0, -25};
		float YawValue = FMath::GetMappedRangeValueClamped(CurveRange, OffsetRange, Amount);
		FVector CameraLoc = CameraComponent->GetRelativeLocation();
		CameraLoc.Y = YawValue;
		CameraComponent->SetRelativeLocation(CameraLoc);
	}
	else if (bIsLeaningRight)
	{
		TRange<float> CurveRange {0, 1};
		TRange<float> RollRange {0, 15};
		float RollValue = FMath::GetMappedRangeValueClamped(CurveRange, RollRange, Amount);
		FRotator CameraRot = Controller->GetControlRotation();
		CameraRot.Roll = RollValue;
		Controller->SetControlRotation(CameraRot);
		//CameraComponent->SetRelativeRotation(CameraRot);
		TRange<float> OffsetRange {0, 25};
		float YawValue = FMath::GetMappedRangeValueClamped(CurveRange, OffsetRange, Amount);
		FVector CameraLoc = CameraComponent->GetRelativeLocation();
		CameraLoc.Y = YawValue;
		CameraComponent->SetRelativeLocation(CameraLoc);
	}
}

void APlayerCharacter::FinishLeanCamera()
{
	if (bIsReversingLeaning)
	{
		bIsLeaningLeft = false;
		bIsLeaningRight = false;
		bIsReversingLeaning = false;
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

void APlayerCharacter::OnEnterLockView()
{
	if (APlayerController* PlayerContr = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerContr->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(LockViewMappingContext, 1);
		}
	}
}

void APlayerCharacter::OnExitLockView()
{
	if (APlayerController* PlayerContr = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerContr->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(LockViewMappingContext);
		}
	}
}


void APlayerCharacter::Initialize()
{
	PlayerController = UGameplayStatics::GetPlayerController(this, 0);


	//HUD
	if (MainHUDClass)
	{
		MainHUDWidget = CreateWidget<UMainHUDWidget>(Cast<AHG_PlayerController>(GetController()), MainHUDClass);
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
	//Footstep timeline
	if (FootstepCurveFloat != nullptr)
	{
		if (TilePhysMaterial == nullptr || GrassPhysMaterial == nullptr || TileFootstepsSoundCue == nullptr || GrassFootstepsSoundCue == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Footsteps objects not set"));
		}
		FootstepTimelineFloat.BindDynamic(this, &APlayerCharacter::UpdateFootstep);
		FootstepTimelineComponent->AddInterpFloat(FootstepCurveFloat, FootstepTimelineFloat);
		FootstepTimelineComponent->SetLooping(true);
		FootstepTimelineComponent->PlayFromStart();
	}
	// Leaning timeline
	if (LeaningCurveFloat != nullptr)
	{
		LeaningTimelineFloat.BindDynamic(this, &APlayerCharacter::LeanCamera);
		LeaningTimelineComponent->AddInterpFloat(LeaningCurveFloat, LeaningTimelineFloat);
		FOnTimelineEventStatic FinishFunc;
		FinishFunc.BindUObject(this, &APlayerCharacter::FinishLeanCamera);
		LeaningTimelineComponent->SetTimelineFinishedFunc(FinishFunc);
	}

	//Flashlight component
	FlashlightComponent->Initialize(this);

	//Game state
	Cast<AHG_GameStateBase>(UGameplayStatics::GetGameState(this))->Initialize(MainHUDWidget);
}