// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NoteExamination.generated.h"

UCLASS()
class UDEMYHORRORGAME_API ANoteExamination : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = Components)
	class UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditAnywhere, Category = Components)
	class UTextRenderComponent* FrontTextRenderComponent;
	UPROPERTY(EditAnywhere, Category = Components)
	class UTextRenderComponent* BackTextRenderComponent;
	
public:	
	// Sets default values for this actor's properties
	ANoteExamination();

	void SetFrontText(FText& Text);
	void SetBackText(FText& Text);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	inline UStaticMeshComponent* GetItemMesh() const { return ItemMesh; }
};
