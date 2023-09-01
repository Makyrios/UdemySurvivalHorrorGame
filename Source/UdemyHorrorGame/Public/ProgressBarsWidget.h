// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProgressBarsWidget.generated.h"

/**
 * 
 */
UCLASS()
class UDEMYHORRORGAME_API UProgressBarsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class USizeBox* SizeBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* Canvas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* BarsVerticalBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class USizeBox* HealthSizeBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class USizeBox* StaminaSizeBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class USizeBox* BatterySizeBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* HealthBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* StaminaBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* BatteryBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* IconsVerticalBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class USizeBox* HealthIconSizeBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class USizeBox* StaminaIconSizeBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class USizeBox* BatteryIconSizeBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* HealthIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* StaminaIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* BatteryIcon;

public:
	void InitializeWidget(class APlayerCharacter* PlayerCharacterRef);

	UFUNCTION(BlueprintCallable)
	float BindHealthBar();
	UFUNCTION(BlueprintCallable)
	float BindStaminaBar();
	UFUNCTION(BlueprintCallable)
	float BindBatteryBar();

protected:
	void NativeConstruct() override;

private:
	class APlayerCharacter* PlayerCharacter;

};
