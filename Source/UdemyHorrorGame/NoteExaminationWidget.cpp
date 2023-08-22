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
#include "Components/Button.h"
#include "Components/Image.h"
#include <string>

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

	Note = NotePickupActor;

	UStaticMeshComponent* ItemMesh = NotePickupActor->StaticMesh;
	UStaticMeshComponent* ExaminationItemMesh = InventoryComponent->GetNoteExaminationActor()->GetItemMesh();
	//ExaminationItemMesh->SetStaticMesh(ItemMesh->GetStaticMesh());
	ExaminationItemMesh->SetRelativeLocation(FVector(NotePickupActor->ExaminationMeshOffset, 0, 0));
	ExaminationItemMesh->SetRelativeRotation(NotePickupActor->ExaminationMeshRotation);

	InventoryComponent->GetNoteExaminationActor()->SetFrontText(NotePickupActor->FrontNoteText);
	InventoryComponent->GetNoteExaminationActor()->SetBackText(NotePickupActor->BackNoteText);
	WidgetText->SetText(ParseNoteText(NotePickupActor->FrontNoteText));

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
	if (IsVisible())
	{
		UE_LOG(LogTemp, Display, TEXT("Close widget"));
		//RemoveFromParent();

		// Enable input
		PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		PlayerContr->ResetIgnoreLookInput();
		PlayerContr->bShowMouseCursor = false;
		PlayerContr->SetInputMode(FInputModeGameOnly());
		PlayerCharacter->bCanOpenInventory = true;

		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UNoteExaminationWidget::OpenTextTranslate()
{
	ReadBackgroundImage->SetVisibility(ESlateVisibility::Visible);
	WidgetText->SetVisibility(ESlateVisibility::Visible);
	XButton->SetVisibility(ESlateVisibility::Visible);
	
	ReadTextButton->SetVisibility(ESlateVisibility::Collapsed);
}

void UNoteExaminationWidget::CloseTextTranslate()
{
	ReadBackgroundImage->SetVisibility(ESlateVisibility::Collapsed);
	WidgetText->SetVisibility(ESlateVisibility::Collapsed);
	XButton->SetVisibility(ESlateVisibility::Collapsed);

	ReadTextButton->SetVisibility(ESlateVisibility::Visible);
}

FText UNoteExaminationWidget::ParseNoteText(FText ParseText)
{
	FString InputString = ParseText.ToString();
	int32 pos = InputString.Find("<br>");

	while (pos != -1)
	{
		InputString.RemoveAt(pos, 4);
		pos = InputString.Find("<br>", ESearchCase::IgnoreCase, ESearchDir::FromStart, pos);
	}

	return FText::FromString(InputString);
}

void UNoteExaminationWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ReadTextButton->OnClicked.AddDynamic(this, &UNoteExaminationWidget::OpenTextTranslate);
	XButton->OnClicked.AddDynamic(this, &UNoteExaminationWidget::CloseTextTranslate);
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
		UE_LOG(LogTemp, Display, TEXT("X: %f\nY: %f\n Z:%f"), InventoryComponent->GetNoteExaminationActor()->GetItemMesh()->GetComponentRotation().Roll,
			InventoryComponent->GetNoteExaminationActor()->GetItemMesh()->GetComponentRotation().Pitch, InventoryComponent->GetNoteExaminationActor()->GetItemMesh()->GetComponentRotation().Yaw);
	}

	StartMouseLocation = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);

	return FReply::Unhandled();
}

FReply UNoteExaminationWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	bRotateMesh = false;
	UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = true;

	if (InventoryComponent->GetNoteExaminationActor()->GetItemMesh()->GetComponentRotation().Yaw >= -90 &&
		InventoryComponent->GetNoteExaminationActor()->GetItemMesh()->GetComponentRotation().Yaw <= 90)
	{
		WidgetText->SetText(ParseNoteText(Note->FrontNoteText));
	}
	else
	{
		WidgetText->SetText(ParseNoteText(Note->BackNoteText));
	}

	return FReply::Handled();
}
