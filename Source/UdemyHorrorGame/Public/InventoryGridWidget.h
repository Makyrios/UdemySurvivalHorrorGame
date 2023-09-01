// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.h"
#include "InventoryMenuWidget.h"
#include "InventoryGridWidget.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYHORRORGAME_API UInventoryGridWidget : public UUserWidget
{
	GENERATED_BODY()

	// ??? Don't want AddSlots function to be public
	friend UInventoryMenuWidget;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UUniformGridPanel* InventoryGridPanel;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	TSubclassOf<UInventorySlotWidget> SlotWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SlotsPerRow = 4;

private:
	void AddSlots(int Amount);

public:
	TArray<class UInventorySlotWidget*> GetSlotsArray() const { return SlotsArray; }

protected:
	void NativeConstruct() override;

	void CreateSlotWidget(int i);

	TArray<class UInventorySlotWidget*> SlotsArray;

	class APlayerCharacter* PlayerCharacter;
	class AHG_PlayerController* PlayerContr;
	UInventoryMenuWidget* InventoryMenu;
};
