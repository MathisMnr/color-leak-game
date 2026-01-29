// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SpeedRun/SpeedRunTimeWidget.h"

#include "Components/TextBlock.h"
#include "Kismet/KismetStringLibrary.h"

void USpeedRunTimeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsRunning)
	{
		Time_ += InDeltaTime;

		SetTime(Time_);
	}
}

void USpeedRunTimeWidget::SetTime(float Time)
{
	TimeText->SetText(FText::FromString(UKismetStringLibrary::TimeSecondsToString(Time)));
}

void USpeedRunTimeWidget::StopTime()
{
	bIsRunning = false;
}

float USpeedRunTimeWidget::GetActualTime()
{
	return Time_;
}
