// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SprintTime.generated.h"

/**
 * 
 */
UCLASS()
class COLORLEAK_API USprintTime : public UUserWidget
{
	GENERATED_BODY()

	float Time = 1.5f;
	float TimeRemaining = 0.f;

	bool bIsTimerActive = false;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar;

	void StartTimer();
};
