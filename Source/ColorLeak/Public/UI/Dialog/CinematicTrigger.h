// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CineCameraActor.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "ColorLeak/ColorLeakGameMode.h"
#include "GameFramework/Actor.h"
#include "CinematicTrigger.generated.h"

// struct CinematicTransform
USTRUCT(BlueprintType)
struct FCinematicTransform
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeInSeconds = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bResetCamAfter = false;
};


UCLASS()
class COLORLEAK_API ACinematicTrigger : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACinematicTrigger();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cinematic")
	TMap<FName, FCinematicTransform> CinematicTransforms;

	FCinematicTransform* ActiveCinematicTransform;

	UFUNCTION()
    void OnDialogTriggered(FName TriggerName);

	UPROPERTY()
	AColorLeakGameMode* GameMode;

	// The CameraActor that will be moved/rotated in the sequence
	UPROPERTY(EditAnywhere)
	ACameraActor* CameraActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cinematic")
	FFrameRate FrameRate = FFrameRate(60,1);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Cinematic")
	ULevelSequence* LevelSequence = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Cinematic")
	ULevelSequencePlayer* SequencePlayer;

	// Play the Level Sequence
	void PlaySequence(FTransform FinalTransform, float Time);

	UFUNCTION()
	void OnPlaybackFinished();
};
