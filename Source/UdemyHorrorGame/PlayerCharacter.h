// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "Components/TimelineComponent.h"
#include "PlayerCharacter.generated.h"

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
	class UMoveComponent* MoveComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Crouch")
	UCurveFloat* CrouchCurveFloat;

	UTimelineComponent* CrouchTimelineComponent;

	//* Input 

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;


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


	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<UUserWidget> MainHUDClass;

private:
	FVector2D LookAxis;
	IGrabbable* GrabbedActor;
	FOnTimelineFloat CrouchTimelineFloat;

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

	void Initialize();
	AActor* LineTrace(float Length);

	UFUNCTION()
	void SetCapsuleHalfHeight(float Amount);

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(EditDefaultsOnly, Category = "Interact")
	float TraceLength = 400;

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

	FORCEINLINE FVector2D GetLookAxis() const { return LookAxis; }
};
