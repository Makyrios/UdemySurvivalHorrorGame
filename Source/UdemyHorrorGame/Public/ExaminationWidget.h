// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExaminationWidget.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYHORRORGAME_API UExaminationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* Canvas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UBackgroundBlur* BackgroundBlur;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* ItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class USizeBox* NameSizeBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class USizeBox* DescriptionSizeBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* NameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* DescriptionText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* DragDetectionImage;

public:
	void InitializeWidget(class UInventoryComponent* InventoryComponentRef);

	void UpdateWidget(int Index);

protected:
	void NativeConstruct() override;

	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;


private:
	class UInventoryComponent* InventoryComponent;
	class APlayerCharacter* PlayerCharacter;

	bool bRotateMesh = false;
	FVector2D StartMouseLocation;

	void CloseExaminationWidget();
};
