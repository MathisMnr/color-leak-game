// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DialogArrowShow.generated.h"

UCLASS()
class COLORLEAK_API ADialogArrowShow : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ArrowMesh;

	UPROPERTY(EditAnywhere)
	FName StartShowArrowTriggerName;

	UPROPERTY(EditAnywhere)
	FName EndShowArrowTriggerName;

	UPROPERTY()
	FTimerHandle ArrowDisplayTimer;

	float ArrowDisplayTimerStart;

public:
	// Sets default values for this actor's properties
	ADialogArrowShow();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FVector InitialArrowPosition;

public:

	UFUNCTION()
	void OnDialogTriggered(FName TriggerName);

	UFUNCTION()
	void MoveArrow();
};
