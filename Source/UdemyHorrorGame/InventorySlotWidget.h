// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemData.h"
#include "InventorySlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYHORRORGAME_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class USizeBox* SizeBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* SlotButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UOverlay* Overlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* SlotImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* Canvas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class USizeBox* AmountSizeBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* AmountText;


	UPROPERTY(EditDefaultsOnly, Category = Icon)
	class UTexture2D* EmptyIcon;
	
	/*FItemData ItemData;
	int Amount;*/

	void UpdateSlot();


	inline int GetIndex() const { return Index; }

private:
	int Index;
	class APlayerCharacter* PlayerCharacter;
	class UInventoryMenuWidget* InventoryMenu;
	FKey LastPressedButton;

protected:
	void NativeConstruct() override;

	FReply NativeOnMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent) override;
	FReply NativeOnMouseButtonUp(const FGeometry& Geometry, const FPointerEvent& MouseEvent) override;

public:
	void InitializeWidget(int ind, UInventoryMenuWidget* InventoryMenuRef);

};
