// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chromarion.h"
#include "GameFramework/Actor.h"
#include "DestructibleBlock.generated.h"

UCLASS()
class COLORLEAK_API ADestructibleBlock : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADestructibleBlock();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Collision")
	class UBoxComponent* BoxCollision;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;

	// Composant de particules
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	UParticleSystemComponent* ParticleSystemComponentDestruction;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "DestructibleBlock")
	void DestroyByJumping(AChromarion* Chromarion);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* BlockDestroySound;

};


