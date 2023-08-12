// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "Components/TimelineComponent.h"
#include "InventoryMenuWidget.h"
#include "PlayerCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FPressedReturn)

class UCameraComponent;
class IGrabbable;
class UTimelineComponent;

UCLASS()
class UDEMYHORRORGAME_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

		//* Components
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

	UPROPERTY(EditAnywhere, Category = "Components")
		class UMoveComponent* MoveComponent;
	UPROPERTY(EditAnywhere, Category = "Components")
		class UInventoryComponent* InventoryComponent;
	UPROPERTY(EditAnywhere, Category = "Components")
		class UFlashlightComponent* FlashlightComponent;
	UPROPERTY(EditAnywhere, Category = "Components")
		class UHealthComponent* HealthComponent;


	UPROPERTY(EditDefaultsOnly, Category = "Crouch")
		UCurveFloat* CrouchCurveFloat;

	UTimelineComponent* CrouchTimelineComponent;

	//* Input 

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* PickupItemMappingContext;

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


	UPROPERTY(EditAnywhere, Category = HUD)
		TSubclassOf<UUserWidget> MainHUDClass;


public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(EditDefaultsOnly, Category = "Interact")
		float TraceLength = 400;

	FPressedReturn PressedReturnEvent;

	bool bIsInventoryOpen = false;
	bool bIsHiding = false;
	bool bCanOpenInventory = true;

	class UExaminationWidget* ExaminationWidget;


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

private:
	FVector2D LookAxis;
	IGrabbable* GrabbedActor;
	FOnTimelineFloat CrouchTimelineFloat;
	APlayerController* PlayerController;
	TArray<class APickupActor_Main*> CurrentPickupItems;

	void Initialize();

	//* Input *//
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interact();
	void StartGrab();
	void StopGrab();
	void ToggleFlashlight();
	void StartSprint();
	void StopSprint();
	void StartCrouch();
	void StopCrouch();
	void PickupItem();
	void Return();

	AActor* LineTrace(float Length);

	UFUNCTION()
	void SetCapsuleHalfHeight(float Amount);

	void HeadBob();

};