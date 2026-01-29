// Fill out your copyright notice in the Description page of Project Settings.


#include "Laser/Receiver/ActiveMechanismLaserReceiver.h"

#include "MovieSceneTracksComponentTypes.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AActiveMechanismLaserReceiver::AActiveMechanismLaserReceiver()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EmissiveSphere = CreateDefaultSubobject<UStaticMeshComponent>("EmissiveSphere");
	EmissiveSphere->SetupAttachment(GetMesh());
}

// Called when the game starts or when spawned
void AActiveMechanismLaserReceiver::BeginPlay()
{
	Super::BeginPlay();

	DynamicMaterial = EmissiveSphere->CreateAndSetMaterialInstanceDynamic(0);

	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue("BaseColor", FromColor(EColorType::Uncolored));
		DynamicMaterial->SetScalarParameterValue("EmissiveFactor", 0.0f);
	}
}

// Called every frame
void AActiveMechanismLaserReceiver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AActiveMechanismLaserReceiver::ActivateMechanism(EColorType Color)
{
	if (bIsOneTimeUse && bIsActive) return;

	UGameplayStatics::PlaySoundAtLocation(this, LaserBeamSound, GetActorLocation());

	Super::ActivateMechanism(Color);

	for (auto & Mechanism : Mechanisms)
	{
		if (Mechanism)
		{
			Mechanism->UpdateState();
		}
	}

	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue("BaseColor", FromColor(Color));
		DynamicMaterial->SetScalarParameterValue("EmissiveFactor", 10.0f);
	}

	bIsActive = true;
}

void AActiveMechanismLaserReceiver::DeactivateMechanism()
{
	if (bIsOneTimeUse) return;

	Super::DeactivateMechanism();

	for (auto & Mechanism : Mechanisms)
	{
		if (Mechanism)
		{
			Mechanism->UpdateState();
		}
	}

	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue("BaseColor", FromColor(EColorType::Uncolored));
		DynamicMaterial->SetScalarParameterValue("EmissiveFactor", 0.0f);
	}

	bIsActive = false;
}
