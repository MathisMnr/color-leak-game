// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "ColorLeak/ColorLeakCharacter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Teleporter.generated.h"

UCLASS()
class COLORLEAK_API ATeleporter : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Teleporter")
	ATeleporter* LinkedTeleporter;

	UPROPERTY(VisibleAnywhere, Category = "Teleporter")
	bool bHasActor = false;

	UPROPERTY(VisibleAnywhere, Category = "Teleporter")
	bool bHasAlreadyTeleport = false;

	UPROPERTY(VisibleAnywhere, Category = "Teleporter")
	bool bTeleportationStarted = false;

	UPROPERTY(VisibleAnywhere, Category = "Teleporter")
	float TeleportationDelay = 4.f;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere)
	AActor* Actor_;

	FTimerHandle TeleporterTimer;

	UPROPERTY(VisibleAnywhere, Category = "FX")
	UNiagaraComponent* TeleportEffectComponent;

public:
	// Sets default values for this actor's properties
	ATeleporter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlap(AActor* MyActor, AActor* OtherActor);

	UFUNCTION()
	void OnEndOverlap(AActor* MyActor, AActor* OtherActor);

	void TeleportCharacter(AActor* Player);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* OverlapTeleporterSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* ChargingTeleportationSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* TeleportingSound;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE bool HasLinkedTeleporter() { return LinkedTeleporter != nullptr; }

	FORCEINLINE bool HasActor() { return bHasActor; }

	FORCEINLINE void SetActor(AActor* Actor) { Actor_ = Actor; }

	FORCEINLINE AActor* GetActor() { return Actor_; }

	void StartEffect();

	void StopEffect(bool bForce = false);
};
