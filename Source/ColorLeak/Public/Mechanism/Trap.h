// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mechanism.h"
#include "Components/TimelineComponent.h"
#include "Trap.generated.h"

UCLASS()
class COLORLEAK_API ATrap : public AMechanism
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATrap();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	UTimelineComponent* TimelineComponent;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* Curve;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	FTransform InitialTransform;
	UPROPERTY(EditAnywhere, Category = "Timeline")
	FTransform EndTransform;

	UFUNCTION()
	void ControlTransform(float Value);

	UFUNCTION()
	void OnTimelineFinished();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void UpdateState() override;
};
