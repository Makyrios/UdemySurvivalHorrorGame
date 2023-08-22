// Fill out your copyright notice in the Description page of Project Settings.


#include "ExaminationWidget.h"
#include "InventoryComponent.h"
#include "Components/TextBlock.h"
#include "Examination.h"
#include <Kismet/GameplayStatics.h>
#include "PlayerCharacter.h"
#include <Blueprint/WidgetLayoutLibrary.h>

void UExaminationWidget::InitializeWidget(UInventoryComponent* InventoryComponentRef)
{
	InventoryComponent = InventoryComponentRef;
}

void UExaminationWidget::UpdateWidget(int Index)
{
	AInventoryItem_Main* Item = InventoryComponent->GetItemClassAtIndex(Index).GetDefaultObject();
	if (Item != nullptr)
	{
		FItemData ItemData = Item->GetItemData();
		NameText->SetText(FText::FromName(ItemData.ItemName));
		DescriptionText->SetText(FText::FromName(ItemData.Description));

		UStaticMeshComponent* ItemMesh = InventoryComponent->GetExaminationActor()->GetItemMesh();
		ItemMesh->SetStaticMesh(ItemData.ExaminationMesh);
		ItemMesh->SetRelativeLocation(FVector(ItemData.ExaminationMeshOffset, 0, 0));
		ItemMesh->SetRelativeRotation(ItemData.ExaminationMeshRotation);

		SetVisibility(ESlateVisibility::Visible);
	}

}

void UExaminationWidget::CloseExaminationWidget()
{
	if (IsVisible())
	{
		UE_LOG(LogTemp, Display, TEXT("Close widget"));
		//RemoveFromParent();
		SetVisibility(ESlateVisibility::Collapsed);
		PlayerCharacter->ToggleInventory();
	}
}

void UExaminationWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	PlayerCharacter->PressedReturnEvent.AddUObject(this, &UExaminationWidget::CloseExaminationWidget);
}

FReply UExaminationWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	bRotateMesh = true;
	StartMouseLocation = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = false;


	return FReply::Handled();
}

FReply UExaminationWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseMove(InGeometry, InMouseEvent);

	FVector2D CurrentMouseLocation = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	FVector2D MouseLocationDiff = CurrentMouseLocation - StartMouseLocation;
	double YawMultiplier = 0.5;
	double RollMultiplier = 0.5;
	FRotator ItemRotation(0, MouseLocationDiff.X * -1 * YawMultiplier, MouseLocationDiff.Y * -1 * RollMultiplier);

	if (bRotateMesh)
	{
		InventoryComponent->GetExaminationActor()->GetItemMesh()->AddWorldRotation(ItemRotation);
	}

	StartMouseLocation = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);

	return FReply::Unhandled();
}

FReply UExaminationWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	bRotateMesh = false;
	UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = true;

	return FReply::Handled();
}
