// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpeedRunTimeWidget.generated.h"

/**
 * 
 */
UCLASS()
class COLORLEAK_API USpeedRunTimeWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TimeText;

	float Time_ = 0.0f;

	bool bIsRunning = true;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetTime(float Time);

	void StopTime();

	float GetActualTime();
};
