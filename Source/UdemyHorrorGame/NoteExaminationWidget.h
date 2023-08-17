// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NoteExaminationWidget.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYHORRORGAME_API UNoteExaminationWidget : public UUserWidget
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
		class UImage* DragDetectionImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* ReadTextButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* ReadTextButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* ReadBackgroundImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* WidgetText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* XButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* XButtonText;

public:
	void InitializeWidget(class UInventoryComponent* InventoryComponentRef);

	void UpdateWidget(class ANote_Main* NotePickupActor);

protected:
	void NativeConstruct() override;

	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;


private:
	class UInventoryComponent* InventoryComponent;
	class APlayerCharacter* PlayerCharacter;
	class APlayerController* PlayerContr;
	class ANote_Main* Note;

	bool bRotateMesh = false;
	FVector2D StartMouseLocation;

	void CloseExaminationWidget();

	UFUNCTION()
	void OpenTextTranslate();
	UFUNCTION()
	void CloseTextTranslate();

	FText ParseNoteText(FText ParseText);
};
