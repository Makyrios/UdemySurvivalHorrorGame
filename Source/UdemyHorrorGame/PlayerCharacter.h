// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;

UCLASS()
class UDEMYHORRORGAME_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	//* Components
	UPROPERTY(EditAnywhere, Category = "Camera")
	class USpringArmComponent* SpringArmComponent;
	UPROPERTY(EditAnywhere, Category = "Camera")
	class UCameraComponent* CameraComponent;

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


	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<UUserWidget> MainHUDClass;

private:
	//* Input *//
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interact();


	AActor* LineTrace(float Length);
	void Initialize();

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

};
