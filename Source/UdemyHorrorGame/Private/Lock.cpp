


// Fill out your copyright notice in the Description page of Project Settings.


#include "Lock.h"
#include "LockDial.h"
#include "Camera/CameraComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerCharacter.h"
#include "Components/WidgetInteractionComponent.h"

// Sets default values
ALock::ALock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LockMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Lock Skeletal Mesh"));
	RootComponent = LockMesh;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	Camera->SetupAttachment(RootComponent);

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	Box->SetupAttachment(RootComponent);
	BoxExtent.Set(32, 32, 32);

	WidgetInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("Widget Interaction Component"));
	WidgetInteraction->InteractionSource = EWidgetInteractionSource::Mouse;

	LockCombination.SetNum(4);
}

// Called when the game starts or when spawned
void ALock::BeginPlay()
{
	Super::BeginPlay();

	Initialize();

	CreateDials();
}

// Called every frame
void ALock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ALock::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	Box->SetRelativeLocation(BoxPosition);
	Box->SetBoxExtent(BoxExtent);
}

void ALock::Initialize()
{
	PlayerContr = UGameplayStatics::GetPlayerController(this, 0);
	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	PlayerCharacter->PressedReturnEvent.AddUObject(this, &ALock::ExitLockView);
	PlayerCharacter->LMBPressedEvent.AddUObject(this, &ALock::MouseClickStarted);
	PlayerCharacter->LMBReleasedEvent.AddUObject(this, &ALock::MouseClickCompleted);

	OnEnterLockView.AddUObject(PlayerCharacter, &APlayerCharacter::OnEnterLockView);
	OnExitLockView.AddUObject(PlayerCharacter, &APlayerCharacter::OnExitLockView);
}

void ALock::CreateDials()
{
	if (LockDialClass != nullptr)
	{
		for (size_t i = 1; i <= 4; i++)
		{
			FName SocketName = FName(TEXT("DialBone") + FString::FromInt(i) + TEXT("Socket"));
			ALockDial* LockDial = Cast<ALockDial>(GetWorld()->SpawnActor(LockDialClass));
			LockDial->Initialize(i);
			LockDial->OnDialRoll.AddUObject(this, &ALock::OnDialRoll);
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
			LockDial->AttachToComponent(LockMesh, AttachmentRules, SocketName);
			Dials.Add(LockDial);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LockDialClass is null"));
	}
}

bool ALock::Interact()
{
	EnterLockView();
	return true;
}

void ALock::EnterLockView()
{
	bIsPlayerUnlocking = true;
	PlayerContr->SetViewTargetWithBlend(Camera->GetOwner(), 0.6);
	PlayerContr->SetIgnoreLookInput(true);
	PlayerContr->bShowMouseCursor = true;
	PlayerContr->SetInputMode(FInputModeGameAndUI());
	PlayerCharacter->GetCharacterMovement()->DisableMovement();
	PlayerCharacter->bCanOpenInventory = false;
	PlayerCharacter->ToggleHUD();
	for (auto& Dial : Dials)
	{
		Dial->ToggleArrows();
	}
	OnEnterLockView.Broadcast();
}

void ALock::ExitLockView()
{
	if (!bIsPlayerUnlocking) return;
	bIsPlayerUnlocking = false;
	PlayerContr->SetViewTargetWithBlend(PlayerCharacter, 0.6);
	PlayerContr->bShowMouseCursor = false;
	PlayerContr->ResetIgnoreLookInput();
	PlayerContr->SetInputMode(FInputModeGameOnly());
	PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	PlayerCharacter->bCanOpenInventory = true;
	PlayerCharacter->ToggleHUD();
	for (auto& Dial : Dials)
	{
		Dial->ToggleArrows();
	}
	OnExitLockView.Broadcast();
}


void ALock::MouseClickStarted()
{
	WidgetInteraction->PressPointerKey(EKeys::LeftMouseButton);
}

void ALock::MouseClickCompleted()
{
	WidgetInteraction->ReleasePointerKey(EKeys::LeftMouseButton);
}

void ALock::OnDialRoll()
{
	if (CheckCode())
	{
		UE_LOG(LogTemp, Display, TEXT("Unlocked"));
		if (UnlockAnimation != nullptr)
		{
			LockMesh->PlayAnimation(UnlockAnimation, false);
			// Exit lock view after unlock animation finished
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() 
				{ 
					ExitLockView();
					// Destroy lock after player exited lock view
					FTimerHandle TimerHandle2;
					GetWorld()->GetTimerManager().SetTimer(TimerHandle2, [this]()
						{
							Destroy();
							for (auto& Dial : Dials)
							{
								Dial->Destroy();
							}
						},
						0.7, false);
				},
				UnlockAnimation->GetPlayLength() + 1, false);
		}
	}
}

bool ALock::CheckCode() const
{
	for (int i = 0; i < 4; i++)
	{
		if (Dials[i]->GetNumber() != LockCombination[i])
		{
			return false;
		}
	}
	return true;
}

