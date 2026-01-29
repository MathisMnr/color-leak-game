// Fill out your copyright notice in the Description page of Project Settings.


#include "Mechanism/Bridge.h"


// Sets default values
ABridge::ABridge()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	BridgeTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("BridgeTimeline"));
}

// Called when the game starts or when spawned
void ABridge::BeginPlay()
{
	Super::BeginPlay();

	InitialPosition = GetActorLocation();

	if (EndPosition.IsZero())
	{
		EndPosition = InitialPosition + FVector(0, 100, 0);
	}

	if (BridgeCurve)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("ControlBridgeTranslation"));

		BridgeTimeline->AddInterpFloat(BridgeCurve, TimelineProgress);

		FOnTimelineEvent TimelineFinished;
		TimelineFinished.BindUFunction(this, FName("OnTimelineFinished"));
		BridgeTimeline->SetTimelineFinishedFunc(TimelineFinished);

		BridgeTimeline->SetLooping(false);  // On ne veut pas que la Timeline boucle
	}
}

void ABridge::ControlBridgeTranslation(float Value)
{
	FVector NewLocation = FMath::Lerp(InitialPosition, EndPosition, Value);
	SetActorLocation(NewLocation);
}

void ABridge::OnTimelineFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("Bridge Timeline Finished"));
}

// Called every frame
void ABridge::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABridge::UpdateState()
{
	Super::UpdateState();

	if (!bIsActivated)
	{
		BridgeTimeline->Play();
	} else
	{
		BridgeTimeline->Reverse();
	}
}