// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DSP/BufferDiagnostics.h"
#include "GameFramework/SaveGame.h"
#include "ColorLeakSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class COLORLEAK_API UColorLeakSaveGame : public USaveGame
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "SaveData")
	double BestTime;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "SaveData")
	TArray<float> BestTimeArray;

	UColorLeakSaveGame();
};
