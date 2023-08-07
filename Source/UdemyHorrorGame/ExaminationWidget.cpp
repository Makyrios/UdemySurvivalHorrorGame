// Fill out your copyright notice in the Description page of Project Settings.


#include "ExaminationWidget.h"
#include "InventoryComponent.h"
#include "Components/TextBlock.h"
#include "Examination.h"
#include <Kismet/GameplayStatics.h>
#include "PlayerCharacter.h"

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

		SetVisibility(ESlateVisibility::Visible);
	}

}

void UExaminationWidget::CloseExaminationWidget()
{
	UE_LOG(LogTemp, Display, TEXT("Close widget"));
	//RemoveFromParent();
	SetVisibility(ESlateVisibility::Collapsed);
	PlayerCharacter->ToggleInventory();
}

void UExaminationWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	PlayerCharacter->PressedReturnEvent.AddUObject(this, &UExaminationWidget::CloseExaminationWidget);
}
