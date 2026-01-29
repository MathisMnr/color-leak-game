// Fill out your copyright notice in the Description page of Project Settings.


#include "Mechanism//Lowering_Plateform.h"
#include "Components/BoxComponent.h"
#include "../ColorLeakCharacter.h"

// Sets default values
ALowering_Plateform::ALowering_Plateform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	BoxCollision->SetupAttachment(StaticMesh);

	Arrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(StaticMesh);

}

// Called when the game starts or when spawned
void ALowering_Plateform::BeginPlay()
{
	Super::BeginPlay();

	this->OnActorBeginOverlap.AddDynamic(this, &ALowering_Plateform::OnOverlap);

	this->OnActorEndOverlap.AddDynamic(this, &ALowering_Plateform::OnEndOverlap);

	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
		CurveTimeline.AddInterpFloat(CurveFloat, TimelineProgress);
		CurveTimeline.SetLooping(false);

		StartLoc = EndLoc = GetActorLocation();
		EndLoc.Z += ZOffset;
	}

}

// Called every frame
void ALowering_Plateform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurveTimeline.TickTimeline(DeltaTime);
}

void ALowering_Plateform::OnOverlap(AActor* MyActor, AActor* OtherActor)
{
	if (OtherActor && bIsActivated)
	{
		OnPlateform += 1;
		CurveTimeline.Play();
	}
}

void ALowering_Plateform::OnEndOverlap(AActor* MyActor, AActor* OtherActor)
{
	if (OtherActor)
	{
		OnPlateform -= 1;
		if (OnPlateform == 0)
		{
			// Démarre un timer de 0.2s avant de remonter la plateforme
			GetWorld()->GetTimerManager().SetTimer(ReverseTimerHandle, this, &ALowering_Plateform::DelayedReverse, 0.7f, false);
		}
	}
}

void ALowering_Plateform::DelayedReverse()
{
	// Vérifie encore qu'il n'y a personne avant d'inverser la timeline
	if (OnPlateform == 0)
	{
		CurveTimeline.Reverse();
	}
}


void ALowering_Plateform::TimelineProgress(float value)
{
	FVector NewLocation = FMath::Lerp(StartLoc, EndLoc, value);
	SetActorLocation(NewLocation);
}

void ALowering_Plateform::UpdateState()
{
	Super::UpdateState();
	if (bIsActivated)
	{		// if Mechanism is reactivated
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("On plateform: %d"), OnPlateform));
		// Récupérer tous les acteurs en overlap avec la BoxCollision
		TArray<AActor*> OverlappingActors;
		BoxCollision->GetOverlappingActors(OverlappingActors, AColorLeakCharacter::StaticClass());

		// Si au moins un personnage est sur la plateforme, on la fait descendre immédiatement
		if (OverlappingActors.Num() > 0)
		{
			CurveTimeline.Play();
		}
	}
	else
	{		// if Mechanism is deactivated
		CurveTimeline.Reverse();		// the plateform goes back to its initial position
	}
}
