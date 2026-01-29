// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Laser/LaserReceiver.h"
#include "ChangeColorBlock.generated.h"

UCLASS()
class COLORLEAK_API AChangeColorBlock : public ALaserReceiver
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AChangeColorBlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ActivateMechanism(EColorType Color) override;
};
