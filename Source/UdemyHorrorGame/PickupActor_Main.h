// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "InventoryItem_Main.h"
#include "PickupActor_Main.generated.h"


UCLASS()
class UDEMYHORRORGAME_API APickupActor_Main : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickupActor_Main();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere)
		class USphereComponent* Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
		class UWidgetComponent* PromptWidgetComponent;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Initialize();

	UPROPERTY(EditAnywhere, Category = Pickup)
		float ArrowPromptLength = 300;
	UPROPERTY(EditAnywhere, Category = Pickup)
		float PickupPromptLength = 200;

	UPROPERTY(EditAnywhere, Category = Pickup)
		float WidgetDistanceAboveMesh = 100;

	UFUNCTION()
		void SphereOnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void SphereOnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Pickup();

protected:
	class APlayerCharacter* PlayerCharacter;
	class APlayerController* PlayerContr;
	class UPickupPromptWidget* PickupPromptWidget;

private:
	bool bIsPlayerOverlap = false;

	UPROPERTY(EditDefaultsOnly, Category = HUD)
		UTexture2D* EPromptImage;

	UPROPERTY(EditDefaultsOnly, Category = HUD)
		UTexture2D* ArrowPromptImage;

	UPROPERTY(EditAnywhere, Category = Item, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AInventoryItem_Main> Item;

	UPROPERTY(EditAnywhere, Category = Item, meta = (AllowPrivateAccess = "true"))
		int Amount;

	void TogglePrompt(bool bCanPickup);
};