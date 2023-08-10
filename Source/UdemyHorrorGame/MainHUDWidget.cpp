// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUDWidget.h"
#include "ProgressBarsWidget.h"

void UMainHUDWidget::InitializeWidget(APlayerCharacter* PlayerCharacterRef)
{
	PlayerCharacter = PlayerCharacterRef;
	ProgressBarsWidget->InitializeWidget(PlayerCharacter);
}
