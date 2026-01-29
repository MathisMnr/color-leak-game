// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ColorTypes.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "LaserReceiver.generated.h"

UCLASS()
class COLORLEAK_API ALaserReceiver : public AActor
{
	GENERATED_BODY()

public:
	ALaserReceiver();

protected:
	virtual void BeginPlay() override;

public:
	virtual void ActivateMechanism(EColorType Color);

	FORCEINLINE UStaticMeshComponent* GetMesh() const { return ReceiverMesh; }

	virtual void DeactivateMechanism();

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ReceiverMesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent* ReceiverBox;
};
