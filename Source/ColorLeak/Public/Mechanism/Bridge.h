// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mechanism.h"
#include "Components/TimelineComponent.h"
#include "Bridge.generated.h"

UCLASS()
class COLORLEAK_API ABridge : public AMechanism
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABridge();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	UTimelineComponent* BridgeTimeline;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* BridgeCurve;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	FVector InitialPosition;
	UPROPERTY(EditAnywhere, Category = "Timeline")
	FVector EndPosition;

	UFUNCTION()
	void ControlBridgeTranslation(float Value);

	UFUNCTION()
	void OnTimelineFinished();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void UpdateState() override;
};
