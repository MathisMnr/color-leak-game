// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Dialog/DialogArrowShow.h"

#include "UI/Dialog/DialogManager.h"


// Sets default values
ADialogArrowShow::ADialogArrowShow()
{
	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));
	ArrowMesh->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ADialogArrowShow::BeginPlay()
{
	Super::BeginPlay();


	InitialArrowPosition = ArrowMesh->GetComponentLocation();

	ArrowMesh->SetVisibility(false);

	UDialogManager::OnDialogTriggered.AddDynamic(this, &ADialogArrowShow::OnDialogTriggered);
}

void ADialogArrowShow::OnDialogTriggered(FName TriggerName)
{
	if (TriggerName == StartShowArrowTriggerName)
	{
		ArrowMesh->SetVisibility(true);
		ArrowDisplayTimerStart = GetWorld()->GetTimeSeconds();
		GetWorldTimerManager().SetTimer(ArrowDisplayTimer, this, &ADialogArrowShow::MoveArrow, .01f, true);
	}
	else if (TriggerName == EndShowArrowTriggerName)
	{
		ArrowMesh->SetVisibility(false);

		GetWorldTimerManager().ClearTimer(ArrowDisplayTimer);
	}
}

void ADialogArrowShow::MoveArrow()
{
	if (!ArrowMesh) return;

	float Time = GetWorld()->GetTimeSeconds() - ArrowDisplayTimerStart;

	float Amplitude = 20.0f;
	float Speed = 2.0f;

	FVector NewPosition = InitialArrowPosition;
	NewPosition.Z += Amplitude * FMath::Sin(Speed * Time);

	ArrowMesh->SetWorldLocation(NewPosition);
}
