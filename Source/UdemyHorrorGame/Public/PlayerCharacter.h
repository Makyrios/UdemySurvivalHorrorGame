// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "Components/TimelineComponent.h"
#include "InventoryMenuWidget.h"
#include "PlayerCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FPressedReturn)
DECLARE_MULTICAST_DELEGATE(FLMBPressed)
DECLARE_MULTICAST_DELEGATE(FLMBReleased)

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FToggleOutlastCamera, bool, bCameraBool);

class UCameraComponent;
class IGrabbable;
class UTimelineComponent;
class UMainHUDWidget;

enum ETraceType
{
	Interact,
	Grab
};

UCLASS()
class UDEMYHORRORGAME_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	// Camera
	UPROPERTY(EditAnywhere, Category = "Camera")
		class USpringArmComponent* SpringArmComponent;
	UPROPERTY(EditAnywhere, Category = "Camera")
		class UCameraComponent* CameraComponent;
	UPROPERTY(EditAnywhere, Category = "Camera")
		class USpotLightComponent* SpotlightComponent;
	UPROPERTY(EditAnywhere, Category = "Camera")
		TSubclassOf<UCameraShakeBase> WalkHeadBob;
	UPROPERTY(EditAnywhere, Category = "Camera")
		TSubclassOf<UCameraShakeBase> RunHeadBob;

	// Components
	UPROPERTY(EditAnywhere, Category = "Components")
		class UMoveComponent* MoveComponent;
	UPROPERTY(EditAnywhere, Category = "Components")
		class UInventoryComponent* InventoryComponent;
	UPROPERTY(EditAnywhere, Category = "Components")
		class UFlashlightComponent* FlashlightComponent;
	UPROPERTY(EditAnywhere, Category = "Components")
		class UHealthComponent* HealthComponent;

	UTimelineComponent* CrouchTimelineComponent;
	UTimelineComponent* FootstepTimelineComponent;
	UTimelineComponent* LeaningTimelineComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Initialization|Curves")
		UCurveFloat* CrouchCurveFloat;
	UPROPERTY(EditDefaultsOnly, Category = "Initialization|Curves")
		UCurveFloat* FootstepCurveFloat;
	UPROPERTY(EditDefaultsOnly, Category = "Initialization|Curves")
		UCurveFloat* LeaningCurveFloat;
	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
		TSubclassOf<UMainHUDWidget> MainHUDClass;
	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
		TSubclassOf<UUserWidget> CameraClass;
	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
		class UPhysicalMaterial* TilePhysMaterial;
	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
		class UPhysicalMaterial* GrassPhysMaterial;
	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
		class USoundCue* TileFootstepsSoundCue;
	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
		class USoundCue* GrassFootstepsSoundCue;


	//* Input 

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* PickupItemMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* LockViewMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* InteractAction;

	/** Grab Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* GrabAction;

	/** Grab Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ToggleFlashlightAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* SprintAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* CrouchAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ToggleInventoryAction;

	/** Pickup Item Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* PickupItemAction;

	/** Return Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ReturnAction;

	/** LMB Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LMBAction;

	/** Leaning Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LeanLeftAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LeanRightAction;

	/** Toggle Camera Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ToggleCameraAction;

	/** Camera Zoom Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ZoomInAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ZoomOutAction;

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(EditDefaultsOnly, Category = "Interact")
		float TraceLength = 400;

	UPROPERTY(EditDefaultsOnly, Category = "Postprocessing")
		float DOFLength = 300;

	UPROPERTY(EditDefaultsOnly, Category = "VideoCamera")
		float MinFOV = 30;
	UPROPERTY(EditDefaultsOnly, Category = "VideoCamera")
		float MaxFOV = 90;

	FPressedReturn PressedReturnEvent;
	FLMBPressed LMBPressedEvent;
	FLMBReleased LMBReleasedEvent;

	UPROPERTY(BlueprintAssignable)
	FToggleOutlastCamera ToggleOutlastCameraEvent;

	bool bIsInventoryOpen = false;
	bool bIsHiding = false;
	bool bCanOpenInventory = true;
	bool bIsLeaningLeft = false;
	bool bIsLeaningRight = false;
	bool bIsReversingLeaning = false;

	class UExaminationWidget* ExaminationWidget;
	class UNoteExaminationWidget* NoteExaminationWidget;

	inline UMoveComponent* GetMoveComponent() const { return MoveComponent; }
	inline UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
	inline UFlashlightComponent* GetFlashlightComponent() const { return FlashlightComponent; }
	inline UHealthComponent* GetHealthComponent() const { return HealthComponent; }
	inline UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	inline USpotLightComponent* GetSpotlightComponent() const { return SpotlightComponent; }
	inline UInputMappingContext* GetInputMappingContext() const { return DefaultMappingContext; }
	inline UInputMappingContext* GetPickupItemMappingContext() const { return PickupItemMappingContext; }

	void AddPickupItem(class APickupActor_Main* PickUpItem);
	void RemovePickupItem(class APickupActor_Main* PickUpItem);

	UFUNCTION(BlueprintCallable)
	void ToggleHUD();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ShortenPlayerCapsule();
	void LengthenPlayerCapsule();

	inline FVector2D GetLookAxis() const { return LookAxis; }

	// Toggle inventory action
	void ToggleInventory();

	void CheckPickupContext();
	void OnEnterLockView();
	void OnExitLockView();

private:
	FVector2D LookAxis;
	IGrabbable* GrabbedActor;
	FOnTimelineFloat CrouchTimelineFloat;
	FOnTimelineFloat FootstepTimelineFloat;
	FOnTimelineFloat LeaningTimelineFloat;
	APlayerController* PlayerController;
	TArray<class APickupActor_Main*> CurrentPickupItems;

	class UMainHUDWidget* MainHUDWidget;
	class UUserWidget* CameraWidget;

	void Initialize();

	//* Input *//
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interact();
	// Grabbing items
	void StartGrab();
	void StopGrab();
	// Flashlight
	void ToggleFlashlight();
	// Sprinting
	void StartSprint();
	void StopSprint();
	// Crouching
	void StartCrouch();
	void StopCrouch();
	// Interact with Pickup actors
	void PickupItem();
	// Return button event
	void Return();
	// Left mouse button events
	void LMBPress();
	void LMBRelease();
	// Leaning
	void StartLeanLeft();
	void StopLeanLeft();
	void StartLeanRight();
	void StopLeanRight();
	// Camera functionality
	void ToggleCamera();
	void ZoomCameraIn();
	void ZoomCameraOut();

	AActor* LineTrace(float Length, ETraceType Type);

	void HeadBob();
	void UpdateDOF();
	void PlayFootstep();

	// Timeline functions
	UFUNCTION()
	void SetCapsuleHalfHeight(float Amount);
	UFUNCTION()
	void UpdateFootstep(float Amount);
	UFUNCTION()
	void LeanCamera(float Amount);
	UFUNCTION()
	void FinishLeanCamera();

};