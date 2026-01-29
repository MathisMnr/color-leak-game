// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PowerUpWidget.generated.h"

/**
 *
 */
UCLASS()
class COLORLEAK_API UPowerUpWidget : public UUserWidget
{
	GENERATED_BODY()

	// text
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PowerLimitText;

	// text
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Minus1Text;

	FVector2D InitialPosition;
	FVector2D TargetPosition;

	float ElapsedTime = 0.f;
	const float Duration = 1.f;

	bool bIsAnimating = false;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn))
	int PowerLimitLeft;

protected:

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void UpdateAnimation();

public:
	void SetPowerLimit(int32 PowerLimit);

	void PowerUsage();
};
