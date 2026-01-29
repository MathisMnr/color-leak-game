// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Laser/LaserReceiver.h"
#include "Mechanism/Mechanism.h"
#include "ActiveMechanismLaserReceiver.generated.h"

UCLASS()
class COLORLEAK_API AActiveMechanismLaserReceiver : public ALaserReceiver
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<AMechanism*> Mechanisms;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* EmissiveSphere;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY(EditAnywhere)
	bool bIsOneTimeUse = false;

	UPROPERTY()
	bool bIsActive = false;

public:
	// Sets default values for this actor's properties
	AActiveMechanismLaserReceiver();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laser")
	USoundWave* LaserBeamSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ActivateMechanism(EColorType Color) override;

	virtual void DeactivateMechanism() override;
};
