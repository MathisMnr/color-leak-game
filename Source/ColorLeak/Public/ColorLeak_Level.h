// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/InputDeviceLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "ColorLeak_Level.generated.h"

/**
 * 
 */
UCLASS()
class COLORLEAK_API AColorLeak_Level : public ALevelScriptActor
{
	GENERATED_BODY()
	
	//Begin play
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APawn> Acryl;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APawn> Chromarion;
};
