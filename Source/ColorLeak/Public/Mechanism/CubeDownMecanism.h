// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mechanism/Mechanism.h"
#include "CubeDownMecanism.generated.h"

/**
 * 
 */
UCLASS()
class COLORLEAK_API ACubeDownMecanism : public AMechanism
{
	GENERATED_BODY()

public:

	ACubeDownMecanism();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path", meta = (ExposeOnSpawn = true, MakeEditWidget = true))
	TArray<FVector> Path;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Components") class UBoxComponent* BoxCollider;
	UPROPERTY(EditDefaultsOnly, Category = "Components") class UStaticMeshComponent* PlatformMesh;


	UPROPERTY(EditDefaultsOnly, Category = "Components") class UInterpToMovementComponent* MovementComponent;

protected:

	virtual void UpdateState() override;
	
};
