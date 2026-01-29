// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InterpToMovementComponent.h"
#include "BlockPath.generated.h"

UCLASS()
class COLORLEAK_API ABlockPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlockPath();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path", meta = (ExposeOnSpawn = true, MakeEditWidget = true))
	TArray<FVector> Path;

	void OpenPath();

	int nbCircleValidate = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxCollision;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Components") 
	class UInterpToMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* WallDownSound;

	int nbCircleTotal = 7;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
