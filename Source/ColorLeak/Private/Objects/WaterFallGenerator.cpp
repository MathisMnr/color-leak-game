#include "Objects/WaterFallGenerator.h"
#include "Objects/Water.h" 
#include "Engine/World.h"
#include "TimerManager.h"

AWaterFallGenerator::AWaterFallGenerator()
{
	PrimaryActorTick.bCanEverTick = false;

	PreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMesh"));
	RootComponent = PreviewMesh;
    
	SpawnInterval = 0.5f;
	ActiveDuration = 5.0f;
	InactiveDuration = 3.0f;
	InitialSpawnDelay = 0.0f;
	WaterLifetime = 2.0f;
	FallingSpeed = 200.0f; 
	SpawnLocationOffset = FVector(0.f, 0.f, 0.f);
}

void AWaterFallGenerator::BeginPlay()
{
	Super::BeginPlay();
	if (bIsActivated)
	{
		StartSpawning();
	}
}

void AWaterFallGenerator::StartSpawning()
{
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AWaterFallGenerator::SpawnWaterBlock, SpawnInterval, true, InitialSpawnDelay);
	GetWorldTimerManager().SetTimer(CycleTimerHandle, this, &AWaterFallGenerator::StopSpawning, ActiveDuration, false);
}

void AWaterFallGenerator::StopSpawning()
{
	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
	GetWorldTimerManager().SetTimer(CycleTimerHandle, this, &AWaterFallGenerator::StartSpawning, InactiveDuration, false);
}

void AWaterFallGenerator::SpawnWaterBlock()
{
	if (WaterBlockClass)
	{
		FVector SpawnLocation = GetActorLocation() + SpawnLocationOffset;
		FRotator SpawnRotation = FRotator::ZeroRotator;
		AWater* SpawnedBlock = GetWorld()->SpawnActor<AWater>(WaterBlockClass, SpawnLocation, SpawnRotation);

		if (SpawnedBlock)
		{
			// Get SpawnedBlock to the PreviewMesh's scale
			FVector PreviewScale = PreviewMesh->GetComponentScale();
			SpawnedBlock->SetActorScale3D(PreviewScale);

			// Destroy after WaterLifeTime sec
			SpawnedBlock->SetLifeSpan(WaterLifetime); 
			// Apply falling speed
			SpawnedBlock->SetFallingSpeed(FallingSpeed);
			SpawnedBlock->bIsDynamic = true;
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Classe BP_Water non assignée dans l'éditeur !"));
	}
}

void AWaterFallGenerator::UpdateState()
{
	AMechanism::UpdateState();
	if (bIsActivated)
	{
		StartSpawning();
	}
	else
	{
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
		GetWorldTimerManager().SetTimer(CycleTimerHandle,[this](){}, InactiveDuration, false);
	}
}
