// Fill out your copyright notice in the Description page of Project Settings.


#include "NoteExaminationWidget.h"
#include "InventoryComponent.h"
#include "Components/TextBlock.h"
#include "NoteExamination.h"
#include <Kismet/GameplayStatics.h>
#include "PlayerCharacter.h"
#include <Blueprint/WidgetLayoutLibrary.h>
#include "Note_Main.h"
#include "GameFramework/CharacterMovementComponent.h"

void UNoteExaminationWidget::InitializeWidget(UInventoryComponent* InventoryComponentRef)
{
	InventoryComponent = InventoryComponentRef;

	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	PlayerCharacter->PressedReturnEvent.AddUObject(this, &UNoteExaminationWidget::CloseExaminationWidget);

	PlayerContr = UGameplayStatics::GetPlayerController(this, 0);
}

void UNoteExaminationWidget::UpdateWidget(ANote_Main* NotePickupActor)
{
	if (NotePickupActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Note pickup actor is null"));
		return;
	}

	UStaticMeshComponent* ItemMesh = NotePickupActor->StaticMesh;
	UStaticMeshComponent* ExaminationItemMesh = InventoryComponent->GetNoteExaminationActor()->GetItemMesh();
	//ExaminationItemMesh->SetStaticMesh(ItemMesh->GetStaticMesh());
	ExaminationItemMesh->SetRelativeLocation(FVector(NotePickupActor->ExaminationMeshOffset, 0, 0));
	ExaminationItemMesh->SetRelativeRotation(NotePickupActor->ExaminationMeshRotation);

	InventoryComponent->GetNoteExaminationActor()->SetFrontText(NotePickupActor->FrontNoteText);
	InventoryComponent->GetNoteExaminationActor()->SetBackText(NotePickupActor->BackNoteText);

	// Disable input
	PlayerCharacter->GetCharacterMovement()->DisableMovement();
	PlayerCharacter->bCanOpenInventory = false;
	PlayerContr->SetIgnoreLookInput(true);
	PlayerContr->bShowMouseCursor = true;
	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(this->GetCachedWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(true);
	PlayerContr->SetInputMode(InputMode);

	SetVisibility(ESlateVisibility::Visible);
}

void UNoteExaminationWidget::CloseExaminationWidget()
{
	UE_LOG(LogTemp, Display, TEXT("Close widget"));
	//RemoveFromParent();

	// Enable input
	PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	PlayerContr->ResetIgnoreLookInput();
	PlayerContr->bShowMouseCursor = false;
	PlayerContr->SetInputMode(FInputModeGameOnly());

	SetVisibility(ESlateVisibility::Collapsed);
}

void UNoteExaminationWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

FReply UNoteExaminationWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	bRotateMesh = true;
	StartMouseLocation = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = false;


	return FReply::Handled();
}

FReply UNoteExaminationWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseMove(InGeometry, InMouseEvent);

	FVector2D CurrentMouseLocation = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	FVector2D MouseLocationDiff = CurrentMouseLocation - StartMouseLocation;
	double YawMultiplier = 0.5;
	double RollMultiplier = 0.5;
	FRotator ItemRotation(0, MouseLocationDiff.X * -1 * YawMultiplier, MouseLocationDiff.Y * -1 * RollMultiplier);

	if (bRotateMesh)
	{
		InventoryComponent->GetNoteExaminationActor()->GetItemMesh()->AddWorldRotation(ItemRotation);
	}

	StartMouseLocation = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);

	return FReply::Unhandled();
}

FReply UNoteExaminationWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	bRotateMesh = false;
	UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = true;

	return FReply::Handled();
}
