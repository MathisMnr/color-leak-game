// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ColorLeakGameInstance.generated.h"


/**
 * Enum to represent the state of a level
 */
UENUM(BlueprintType)
enum class ELevels : uint8
{
    Red_1        UMETA(DisplayName = "Red 1"),
    Red_2        UMETA(DisplayName = "Red 2"),
    Blue_1       UMETA(DisplayName = "Blue 1"),
    Blue_2       UMETA(DisplayName = "Blue 2"),
    LongLevel_1  UMETA(DisplayName = "Long Level 1"),
    Yellow_1     UMETA(DisplayName = "Yellow 1"),
    Yellow_2     UMETA(DisplayName = "Yellow 2"),
    Orange_1     UMETA(DisplayName = "Orange 1"),
    Orange_2     UMETA(DisplayName = "Orange 2"),
    Green_1      UMETA(DisplayName = "Green 1"),
    Green_2      UMETA(DisplayName = "Green 2"),
    Purple_1     UMETA(DisplayName = "Purple 1"),
    Purple_2     UMETA(DisplayName = "Purple 2"),
};

/**
 * 
 */
UCLASS()
class COLORLEAK_API UColorLeakGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable)
    static FText GetLevelName(ELevels Level);

    UFUNCTION(BlueprintCallable)
    static FName GetDevLevelName(ELevels Level);

    UFUNCTION(BlueprintCallable)
    static ELevels GetPreviousLevel(ELevels CurrentLevel);

    UFUNCTION(BlueprintCallable)
    static ELevels GetLevelByName(FName LevelName);

    // can see in blueprint
    UPROPERTY(BlueprintReadWrite, Category = "Levels")
    TArray<ELevels> FinishedLevels;

    UPROPERTY(BlueprintReadWrite, Category = "Levels")
    bool bIsFirstTimeLobby = true;

    UFUNCTION(BlueprintCallable, Category = "Levels")
    void FinishLevel(ELevels Level);

    UFUNCTION(BlueprintCallable, Category = "Levels")
    bool IsLevelFinished(ELevels Level);

    UFUNCTION(BlueprintCallable, Category = "Levels")
    TArray<ELevels> GetFinishedLevels();

    // Speedrun
    UPROPERTY(BlueprintReadWrite, Category = "Speedrun")
    bool bIsSpeedRun = false;

    UPROPERTY(BlueprintReadWrite, Category = "Speedrun")
    float TotalIGT = 0.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Speedrun")
    FDateTime RealStartTime = FDateTime::UtcNow();

    UPROPERTY(BlueprintReadWrite, Category = "Speedrun")
    FDateTime RealEndTime;

    UPROPERTY(BlueprintReadWrite, Category = "Speedrun")
    int nbDeathAcryl = 0;

    UPROPERTY(BlueprintReadWrite, Category = "Speedrun")
    int nbDeathChromarion = 0;

    UFUNCTION(BlueprintCallable, Category = "Levels")
    void StartSpeedRun() {
        bIsSpeedRun = true;
        TotalIGT = 0.0f;
        RealStartTime = FDateTime::UtcNow();
    }

    UFUNCTION(BlueprintCallable, Category = "Levels")
    float GetRealTimeSeconds() const {
        return (RealEndTime - RealStartTime).GetTotalSeconds();
    }

    UFUNCTION(BlueprintCallable, Category = "Levels")
    void SetRealEndTime() {
        RealEndTime = FDateTime::UtcNow();
    }

    UFUNCTION(BlueprintCallable, Category = "Levels")
    void AddIGT(float Time) {
        TotalIGT += Time;
    }
};
