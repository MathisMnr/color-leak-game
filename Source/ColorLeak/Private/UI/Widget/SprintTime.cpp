// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/SprintTime.h"

#include "Components/ProgressBar.h"

void USprintTime::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsTimerActive)
	{
		TimeRemaining -= InDeltaTime;

		if (TimeRemaining <= 0)
		{
			TimeRemaining = 0;
			bIsTimerActive = false;
		}

		ProgressBar->SetPercent(TimeRemaining / Time);
	}
}

void USprintTime::StartTimer()
{
	TimeRemaining = Time;
	bIsTimerActive = true;
	ProgressBar->SetPercent(1.f);
}
