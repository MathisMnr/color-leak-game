// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StartDialog.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COLORLEAK_API UStartDialog : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	bool bHasAlreadyDialog = false;

	UPROPERTY(EditAnywhere)
	bool bIsDialog = false;

	UPROPERTY(EditAnywhere)
	FName DialogName;

public:
	// Sets default values for this component's properties
	UStartDialog();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// OnOverlap function
	UFUNCTION()
	void OnOverlap(AActor* MyActor, AActor* OtherActor);
};
