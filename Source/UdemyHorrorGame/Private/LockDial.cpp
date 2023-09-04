// Fill out your copyright notice in the Description page of Project Settings.


#include "LockDial.h"
#include "Components/WidgetComponent.h"
#include "ArrowWidget.h"
#include "Components/Button.h"

// Sets default values
ALockDial::ALockDial()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	RootComponent = Scene;

	DialMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	DialMesh->SetupAttachment(Scene);
	DialMesh->SetCollisionProfileName(FName("OverlapAll"));

	RotateTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("Rotate Timeline Component"));

	TopArrowComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Top Arrow Component"));
	BottomArrowComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Bottom Arrow Component"));

	TopArrowComponent->SetWidgetClass(LoadClass<UUserWidget>(NULL, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Lock/WBP_ArrowWidget.WBP_ArrowWidget_C'")));
	TopArrowComponent->SetupAttachment(Scene);
	TopArrowComponent->SetVisibility(false);
	TopArrowComponent->CastShadow = false;
	BottomArrowComponent->SetWidgetClass(LoadClass<UUserWidget>(NULL, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Lock/WBP_ArrowWidget.WBP_ArrowWidget_C'")));
	BottomArrowComponent->SetupAttachment(Scene);
	BottomArrowComponent->SetVisibility(false);
	BottomArrowComponent->CastShadow = false;
}

void ALockDial::Initialize(int ind)
{
	Index = ind;

	// Set rotation timeline
	if (RotateCurveFloat != nullptr)
	{
		OnTimelineFloat.BindDynamic(this, &ALockDial::Rotate);
		RotateTimelineComponent->AddInterpFloat(RotateCurveFloat, OnTimelineFloat);

		FOnTimelineEventStatic OnTimelineFinishedCallback;
		OnTimelineFinishedCallback.BindUObject(this, &ALockDial::FinishRotate);
		RotateTimelineComponent->SetTimelineFinishedFunc(OnTimelineFinishedCallback);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Rotate curve floats are null"));
	}

	// Create and bind UI arrows
	UArrowWidget* TopArrow = Cast<UArrowWidget>(TopArrowComponent->GetWidget());
	UArrowWidget* BottomArrow = Cast<UArrowWidget>(BottomArrowComponent->GetWidget());

	if (TopArrow != nullptr)
	{
		TopArrow->ArrowButton->OnPressed.AddDynamic(this, &ALockDial::RollNumberUp);
	}
	if (BottomArrow != nullptr)
	{
		BottomArrow->ArrowButton->OnPressed.AddDynamic(this, &ALockDial::RollNumberDown);
	}

	// Set random numbers on dial on spawn
	if (bRandomDial)
	{
		// Set random rotation
		int RandomRotation = FMath::RandRange(0, 9) * 36;
		FRotator RandomRotator(0, 0, RandomRotation);
		DialMesh->SetRelativeRotation(RandomRotator);
		DialRotation = RandomRotator.Roll;
		// Find according number to roll
		Number = FindNumber(DialRotation);
	}
	else
	{
		DialRotation = DialMesh->GetRelativeRotation().Roll;
		Number = FindNumber(DialRotation);
	}
}

void ALockDial::RollNumberUp()
{
	UE_LOG(LogTemp, Display, TEXT("Roll up"));
	if (bCanRotate)
	{
		bCanRotate = false;
		bIsForward = true;
		RotateTimelineComponent->PlayFromStart();
	}
}

void ALockDial::RollNumberDown()
{
	UE_LOG(LogTemp, Display, TEXT("Roll down"));
	if (bCanRotate)
	{
		bCanRotate = false;
		bIsForward = false;
		RotateTimelineComponent->PlayFromStart();
	}
}

void ALockDial::ToggleArrows()
{
	if (!TopArrowComponent->IsVisible())
	{
		TopArrowComponent->SetVisibility(true);
		BottomArrowComponent->SetVisibility(true);
	}
	else
	{
		TopArrowComponent->SetVisibility(false);
		BottomArrowComponent->SetVisibility(false);
	}
}

// Called when the game starts or when spawned
void ALockDial::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void ALockDial::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Display, TEXT("Can Rotate: %s"), bCanRotate ? TEXT("TRUE") : TEXT("FALSE"));
}

void ALockDial::Rotate(float Value)
{
	if (bIsForward)
	{
		TRange<float> CurveRange {0, 1};
		TRange<float> RotationRange {0, 36};
		Value = FMath::GetMappedRangeValueClamped(CurveRange, RotationRange, Value);
		RotationRate = DialRotation + Value;
		FRotator Rotator = FRotator(0, 0, RotationRate);
		DialMesh->SetRelativeRotation(Rotator);
	}
	else
	{
		TRange<float> CurveRange {0, 1};
		TRange<float> RotationRange {0, -36};
		Value = FMath::GetMappedRangeValueClamped(CurveRange, RotationRange, Value);
		RotationRate = DialRotation + Value;
		FRotator Rotator = FRotator(0, 0, RotationRate);
		DialMesh->SetRelativeRotation(Rotator);
	}
}

void ALockDial::FinishRotate()
{
	if (bIsForward)
	{
		DialRotation = RotationRate;
		if (DialRotation >= 360)
		{
			DialRotation = 0;
			DialMesh->SetRelativeRotation(FRotator(0, 0, 0));
		}
		Number = FindNumber(DialRotation);
		bCanRotate = true;
		OnDialRoll.Broadcast();
	}
	else
	{
		DialRotation = RotationRate;
		if (DialRotation <= 0)
		{
			DialRotation += 360;
			DialMesh->SetRelativeRotation(FRotator(0, 0, DialRotation));
		}
		Number = FindNumber(DialRotation);
		bCanRotate = true;
		OnDialRoll.Broadcast();
	}
}

int ALockDial::FindNumber(float Roll) const
{
	TRange<float> R1 {0, 324};
	TRange<float> R2 {0, 9};
	float Result = FMath::GetMappedRangeValueClamped<float>(R1, R2, Roll);
	return FMath::RoundToInt(Result);
}

