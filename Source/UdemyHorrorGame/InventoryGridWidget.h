// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.h"
#include "InventoryGridWidget.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYHORRORGAME_API UInventoryGridWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UUniformGridPanel* InventoryGridPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	TSubclassOf<UInventorySlotWidget> SlotWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SlotsPerRow = 4;

	TArray<class UInventorySlotWidget*> GetSlotsArray() const { return SlotsArray; }

protected:
	void NativeConstruct() override;

	TArray<class UInventorySlotWidget*> SlotsArray;


};
