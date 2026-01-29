// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UI/Dialog/DialogWidget.h"
#include "UI/SpeedRun/SpeedRunTimeWidget.h"
#include "ColorLeakGameMode.generated.h"

UCLASS(minimalapi)
class AColorLeakGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AColorLeakGameMode();

	// Call when characters died
	void RespawnPlayer(AController* Controller, TSubclassOf<APawn> CharacterClass, FName SpawnTag);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	bool respawn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", meta = (AllowPrivateAccess = "true"))
	bool dead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", meta = (AllowPrivateAccess = "true"))
	bool bUpdateCameraEveryTick = true;

	UFUNCTION(BlueprintCallable)
	void StartDialog(FName DialogName);

	UFUNCTION(BlueprintCallable)
	void AdvanceDialog();

	UFUNCTION(BlueprintCallable)
	void SetDialogActive(bool bIsActive);

	UFUNCTION(BlueprintCallable)
	void SetPlayerMovementEnabled(bool bEnable);

	UFUNCTION(BlueprintCallable)
	void AddIGT();

	UFUNCTION(BlueprintCallable)
	void StartSpeedRun();

	UFUNCTION(BlueprintCallable)
	float GetFinishTime();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	int nbDeathAcryl = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	int nbDeathChromarion = 0;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UDialogWidget* DialogWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UDialogWidget> DialogWidgetClass;

	bool bIsDialogActive = false;

	UPROPERTY()
	USpeedRunTimeWidget* SpeedRunTimeWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USpeedRunTimeWidget> SpeedRunTimeWidgetClass;
};
