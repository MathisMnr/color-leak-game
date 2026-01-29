// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ColorTypes.h"
#include "LaserReceiver.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "ColorLeak/ColorLeakCharacter.h"
#include "Components/BoxComponent.h"
#include "Interface/PaintableInterface.h"
#include "LaserEmitter.generated.h"

UCLASS()
class COLORLEAK_API ALaserEmitter : public AActor, public IPaintableInterface
{
	GENERATED_BODY()

public:
	ALaserEmitter();

	void HideLaser(int FromBounceCount = 0);

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void TimerCallback();

private:
	ALaserReceiver* FireLaser(FVector Start, FVector Direction, int32 BounceCount, AActor* OwnerActor, EColorType Color);

	virtual void SetCurrentColor(EColorType newColor) override;

	UPROPERTY(EditAnywhere, Category = "Laser")
	int32 MaxReflections = 5;  // Max number of reflections

	UPROPERTY(EditAnywhere, Category = "Laser")
	float LaserLength = 1000.f; // Max length of each laser segment

	UPROPERTY(EditAnywhere, Category = "Laser")
	float RefreshRate = 0.2f; // Refresh rate in seconds

	UPROPERTY(EditAnywhere, Category = "Laser")
	EColorType ColorType = EColorType::Uncolored;

	UPROPERTY(EditAnywhere, Category = "Laser")
	bool bIsStatic = false;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* LaserMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* EmissiveSphereMesh;
	
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere)
	UArrowComponent* ArrowComponent;

	UPROPERTY(EditAnywhere, Category = "Laser")
	UNiagaraSystem* LaserSystem;

	UPROPERTY(VisibleAnywhere, Category = "Laser")
	TArray<UNiagaraComponent*> LaserComponents;

	UPROPERTY()
	ALaserReceiver* LaserReceiver;

	bool bHitPlayer = false;
	bool bHitPlayerLastTick = false;

	UPROPERTY()
	AColorLeakCharacter* HitPlayer = nullptr;

	UPROPERTY()
	AColorLeakCharacter* HitPlayerLastTick = nullptr;

	UPROPERTY(EditAnywhere, Category = "Laser")
	UForceFeedbackEffect* ForceFeedbackEffectHitLaser;
};
