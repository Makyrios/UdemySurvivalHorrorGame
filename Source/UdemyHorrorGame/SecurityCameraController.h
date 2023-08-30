// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "SecurityCameraController.generated.h"

UCLASS()
class UDEMYHORRORGAME_API ASecurityCameraController : public AActor, public IInteractable
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = Components)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly, Category = Input)
		class UInputMappingContext* CameraMappingContext;
	UPROPERTY(EditDefaultsOnly, Category = Input)
		class UInputAction* ShowNextCameraAction;
	UPROPERTY(EditDefaultsOnly, Category = Input)
		class UInputAction* ShowPrevCameraAction;

	
public:	
	// Sets default values for this actor's properties
	ASecurityCameraController();
	
	bool Interact() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditDefaultsOnly, Category = Initialization)
	TSubclassOf<AActor> SecurityCameraClass;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TArray<class ASecurityCamera*> SecurityCameras;
	int CurrentCameraIndex = 0;

	bool bIsViewingCameras = false;

	void EnterCameraController();
	void ExitCameraController();

	void ShowNextCamera();
	void ShowPrevCamera();

	void ViewThroughCamera();

	APlayerController* PlayerContr;
	class APlayerCharacter* PlayerCharacter;
};
