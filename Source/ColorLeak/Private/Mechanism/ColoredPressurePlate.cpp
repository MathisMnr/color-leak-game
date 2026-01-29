// Fill out your copyright notice in the Description page of Project Settings.

#include "Mechanism/ColoredPressurePlate.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequence.h"
#include "EngineUtils.h"
#include "Mechanism/Mechanism.h"
#include "Components/AudioComponent.h"
#include "Objects/ColoredBox.h"


AColoredPressurePlate::AColoredPressurePlate()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create StaticMeshComponent and Attach to BoxComponent
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	RootComponent = BoxCollision;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(BoxCollision);
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->VolumeMultiplier = 100.f;
	AudioComponent->bAutoActivate = false;
}

void AColoredPressurePlate::BeginPlay()
{
	Super::BeginPlay();

	// Bind function OnActorBeginOverlap with your class function OnOverlap
	this->OnActorBeginOverlap.AddDynamic(this, &AColoredPressurePlate::OnOverlap);

	// Bind function OnActorBeginOverlap with your class function EndOverlap
	this->OnActorEndOverlap.AddDynamic(this, &AColoredPressurePlate::OnEndOverlap);

	UpdateColor(PlateColor);

	InitialScale = StaticMesh->GetComponentScale();
}

void AColoredPressurePlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AColoredPressurePlate::OnOverlap(AActor* MyActor, AActor* OtherActor)
{
	if (IsValidActor(OtherActor))
	{
		OverlappingValidActors++;
		
		if (!bIsGoingDown)
		{
			ChangeState(false);
			bIsGoingDown = true;
			StartPlateAnimation(false); 
		}
	}
}


void AColoredPressurePlate::OnEndOverlap(AActor* MyActor, AActor* OtherActor)
{
	if (IsValidActor(OtherActor))
	{
		OverlappingValidActors--;

		if (OverlappingValidActors <= 0)
		{
			ChangeState(false);
			bIsGoingDown = false;
			StartPlateAnimation(true); 
		}
	}
}


bool AColoredPressurePlate::IsValidActor(AActor* OtherActor)
{
	if (auto Chromarion = Cast<AChromarion>(OtherActor))
	{
		return Chromarion->GetCurrentColor() == PlateColor;
	}
	if (auto Acryl = Cast<AAcryl>(OtherActor))
	{
		return Acryl->GetColorBrush("Socket_Brush") == PlateColor;
	}
	if (auto ColoredBox = Cast<AColoredBox>(OtherActor))
	{
		return ColoredBox->BoxColor == PlateColor;
	}
	
	return false;
}


void AColoredPressurePlate::ChangeState(bool bIsEmiter)
{
	// Change the state of the Plate
	bIsActivated = !bIsActivated;

	// Update the state of the Mechanism linked to the Plate
	for (TActorIterator<AMechanism> It(GetWorld()); It; ++It)
	{
		AMechanism* Mechanism = *It;
		if (Mechanism && Mechanism->LinkedPlate == this)
		{
			Mechanism->UpdateState();
			// log the state of the mechanism
			//FString StateM = Mechanism->bIsActivated ? "Activated" : "Deactivated";
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Mechanism is %s"), *StateM));

		}
	}

	if (bIsEmiter) {
		// Play the sound
		if (PlateActivationSound && AudioComponent)
		{
			AudioComponent->Play();
		}
	}
}

bool AColoredPressurePlate::GetPlateState()
{
	return bIsActivated;
}

void AColoredPressurePlate::UpdateColor(EColorType NewColor)
{
	FString ColorName;
	PlateColor = NewColor;
	
	switch (NewColor)
	{
	case EColorType::Red:
		ColorName = TEXT("Red");
		break;
	case EColorType::Yellow:
		ColorName = TEXT("Yellow");
		break;
	case EColorType::Blue:
		ColorName = TEXT("Blue");
		break;
	case EColorType::Green:
		ColorName = TEXT("Green");
		break;
	case EColorType::Orange:
		ColorName = TEXT("Orange");
		break;
	case EColorType::Purple:
		ColorName = TEXT("Purple");
		break;
	case EColorType::Uncolored:
		ColorName = TEXT("UnColored");
		break;
	default:
		ColorName = TEXT("UnColored");
		break;
	}

	FString MainMaterialPath = FString::Printf(TEXT("Material'/Game/Characters/Chromarion/Materials/%s/M_Main.M_Main'"), *ColorName);
	UMaterialInterface* Main = LoadObject<UMaterialInterface>(nullptr, *MainMaterialPath);

	FString SecondaryMaterialPath = FString::Printf(TEXT("Material'/Game/Characters/Chromarion/Materials/%s/M_Secondary.M_Secondary'"), *ColorName);
	UMaterialInterface* Secondary = LoadObject<UMaterialInterface>(nullptr, *SecondaryMaterialPath);

	if (Main && Secondary)
	{
		// Apply material to ColoredBox
		StaticMesh->SetMaterial(0, Main);
		StaticMesh->SetMaterial(1, Secondary);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Scale Animation

void AColoredPressurePlate::StartPlateAnimation(bool bReverse)
{
	GetWorld()->GetTimerManager().ClearTimer(PlateMoveTimerHandle);

	ElapsedTime = 0.0f;
	TargetScale = InitialScale;
	if (!bReverse)
	{
		TargetScale.Z *= 0.5f; 
	}

	// Play the sound
	if (PlateActivationSound && AudioComponent)
	{
		if (bReverse)
		{
			AudioComponent->SetSound(PlateDeactivationSound);
		}
		else 
		{
			AudioComponent->SetSound(PlateActivationSound);
		}
		AudioComponent->Play();
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Play sound %s"), *AudioComponent->GetSound()->GetName()));
	}

	GetWorld()->GetTimerManager().SetTimer(PlateMoveTimerHandle, this, &AColoredPressurePlate::UpdatePlateScale, AnimationStepTime, true);
}

void AColoredPressurePlate::UpdatePlateScale()
{
	ElapsedTime += AnimationStepTime;
	float Alpha = FMath::Clamp(ElapsedTime / AnimationDuration, 0.0f, 1.0f);

	FVector NewScale = FMath::Lerp(StaticMesh->GetComponentScale(), TargetScale, Alpha);
	StaticMesh->SetWorldScale3D(NewScale);

	if (Alpha >= 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(PlateMoveTimerHandle);
	}
}

