// Fill out your copyright notice in the Description page of Project Settings.


#include "Mechanism/Trap.h"

#include "Kismet/KismetMathLibrary.h"


// Sets default values
ATrap::ATrap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent"));
}

// Called when the game starts or when spawned
void ATrap::BeginPlay()
{
	Super::BeginPlay();

	InitialTransform = GetActorTransform();

	if (EndTransform.Equals(FTransform()))
	{
		EndTransform = FTransform(InitialTransform.GetLocation() + FVector(0, 100, 0));
	}

	if (Curve)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("ControlTransform"));

		TimelineComponent->AddInterpFloat(Curve, TimelineProgress);

		FOnTimelineEvent TimelineFinished;
		TimelineFinished.BindUFunction(this, FName("OnTimelineFinished"));
		TimelineComponent->SetTimelineFinishedFunc(TimelineFinished);

		TimelineComponent->SetLooping(false);  // On ne veut pas que la Timeline boucle
	}
}

void ATrap::ControlTransform(float Value)
{
	FTransform NewTransform = UKismetMathLibrary::TLerp(InitialTransform, EndTransform, Value);
	SetActorTransform(NewTransform);
}

void ATrap::OnTimelineFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("Timeline Finished"));
}

// Called every frame
void ATrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATrap::UpdateState()
{
	Super::UpdateState();

	if (!bIsActivated)
	{
		TimelineComponent->Play();
	} else
	{
		TimelineComponent->Reverse();
	}
}

