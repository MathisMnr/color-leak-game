// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/PowerUpWidget.h"

#include "Components/TextBlock.h"

void UPowerUpWidget::UpdateAnimation()
{
	float Alpha = ElapsedTime / Duration;

	if (Alpha >= 1.f)
	{
		Minus1Text->SetVisibility(ESlateVisibility::Hidden);
		bIsAnimating = false;
	}
	else
	{
		FVector2D NewPosition = FMath::Lerp(InitialPosition, TargetPosition, Alpha);
		Minus1Text->SetRenderTranslation(NewPosition);
	}
}

void UPowerUpWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitialPosition = Minus1Text->GetRenderTransform().Translation;
	TargetPosition = InitialPosition + FVector2D(0.f, 20.f);

	Minus1Text->SetVisibility(ESlateVisibility::Hidden);
}

void UPowerUpWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsAnimating)
	{
		ElapsedTime += InDeltaTime;
		UpdateAnimation();
	}
}

void UPowerUpWidget::SetPowerLimit(int32 PowerLimit)
{
	//PowerLimitText->SetText(FText::FromString(FString::Printf(TEXT("x %d"), PowerLimit)));
	if (PowerLimit < 0)
	{
		PowerLimitLeft = 0;
	} else
	{
		PowerLimitLeft = PowerLimit;
	}
}

void UPowerUpWidget::PowerUsage()
{
	Minus1Text->SetRenderTranslation(InitialPosition);
	Minus1Text->SetVisibility(ESlateVisibility::Visible);

	ElapsedTime = 0.f;
	bIsAnimating = true;
}
