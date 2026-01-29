#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mechanism/Mechanism.h"
#include "WaterFallGenerator.generated.h"

UCLASS()
class COLORLEAK_API AWaterFallGenerator : public AMechanism
{
	GENERATED_BODY()
    
public:    
	AWaterFallGenerator();

protected:
	virtual void BeginPlay() override;

	void StartSpawning();
	void StopSpawning();
	void SpawnWaterBlock();

	UPROPERTY(EditAnywhere, Category = "Water Generator") UStaticMeshComponent* PreviewMesh;

	UPROPERTY(EditAnywhere, Category = "Waterfall Settings") TSubclassOf<AActor> WaterBlockClass;
	
	UPROPERTY(EditAnywhere, Category = "Waterfall Settings") float SpawnInterval; 
	UPROPERTY(EditAnywhere, Category = "Waterfall Settings") float InitialSpawnDelay;
	
	UPROPERTY(EditAnywhere, Category = "Waterfall Settings") float ActiveDuration; 
	UPROPERTY(EditAnywhere, Category = "Waterfall Settings") float InactiveDuration; 
	UPROPERTY(EditAnywhere, Category = "Waterfall Settings") float MaxScaleZ; 
	
	UPROPERTY(EditAnywhere, Category = "Waterfall Settings") float WaterLifetime; 
	UPROPERTY(EditAnywhere, Category = "Waterfall Settings") float FallingSpeed; 
	
	UPROPERTY(EditAnywhere, Category = "Waterfall Settings") FVector SpawnLocationOffset;

	float CurrentScaleZ;
	float GrowthRate;
	bool bIsGrowing;
	
	virtual void UpdateState() __override;

private:
	FTimerHandle SpawnTimerHandle;
	FTimerHandle CycleTimerHandle;
};
