// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Dialog/StartDialog.h"

#include "ColorLeak/ColorLeakCharacter.h"
#include "ColorLeak/ColorLeakGameMode.h"


// Sets default values for this component's properties
UStartDialog::UStartDialog()
{
}

// Called when the game starts
void UStartDialog::BeginPlay()
{
	Super::BeginPlay();

	if (auto OwnerActor = GetOwner())
	{
		OwnerActor->OnActorBeginOverlap.AddDynamic(this, &UStartDialog::OnOverlap);
	}
}

void UStartDialog::OnOverlap(AActor* MyActor, AActor* OtherActor)
{
	if (!MyActor || !OtherActor) return;
	if (bHasAlreadyDialog || !bIsDialog) return;

	if (auto Player = Cast<AColorLeakCharacter>(OtherActor))
	{
		// get the GM
		if (auto GM = Cast<AColorLeakGameMode>(GetWorld()->GetAuthGameMode()))
		{
			bHasAlreadyDialog = true;
			GM->StartDialog(DialogName);
		}
	}
}
