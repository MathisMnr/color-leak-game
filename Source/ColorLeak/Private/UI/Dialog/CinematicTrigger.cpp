// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Dialog/CinematicTrigger.h"

#include "ColorLeak/ColorLeakGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Dialog/DialogManager.h"
#include "Objects/Easel/EaselActor.h"
#include "Sequence/SequenceManager.h"


ACinematicTrigger::ACinematicTrigger()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACinematicTrigger::BeginPlay()
{
	Super::BeginPlay();

	UDialogManager::OnDialogTriggered.AddDynamic(this, &ACinematicTrigger::OnDialogTriggered);

	if (auto GM = Cast<AColorLeakGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		GameMode = GM;
	}
}

void ACinematicTrigger::OnDialogTriggered(FName TriggerName)
{
	if (auto CT = CinematicTransforms.Find(TriggerName))
	{
		if (CT)
		{
			if (SequencePlayer)
			{
				SequencePlayer->Stop();
			}

			if (GameMode)
			{
				GameMode->bUpdateCameraEveryTick = false;
			}

			ActiveCinematicTransform = CT;
			PlaySequence(CT->Transform, CT->TimeInSeconds);
		}
	}

	if (TriggerName == "EndDialog")
	{
		if (SequencePlayer)
		{
			SequencePlayer->Stop();
			SequencePlayer->OnFinished.RemoveDynamic(this, &ACinematicTrigger::OnPlaybackFinished);
			SequencePlayer = nullptr;
			ActiveCinematicTransform = nullptr;
		}
		if (GameMode)
		{
			GameMode->bUpdateCameraEveryTick = true;
		}
	}
}

void ACinematicTrigger::PlaySequence(FTransform FinalTransform, float Time)
{
	UMovieScene* MovieScene;
	LevelSequence = SequenceManager::CreateSequence(MovieScene, FrameRate, Time);

	FGuid BindingID;
	SequenceManager::BindObjectToSequence(MovieScene, BindingID, CameraActor, LevelSequence);

	UMovieScene3DTransformSection* TransformSection;
	SequenceManager::AddTransformTrack(MovieScene, BindingID, TransformSection);

	SequenceManager::AddKeyFrames(MovieScene, TransformSection, CameraActor->GetTransform(), 0.0);

	SequenceManager::AddKeyFrames(MovieScene, TransformSection, FinalTransform, Time);

	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	PlaybackSettings.FinishCompletionStateOverride = EMovieSceneCompletionModeOverride::ForceKeepState;
	ALevelSequenceActor* SequenceActor = nullptr;
	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), LevelSequence, PlaybackSettings, SequenceActor);

	if (SequencePlayer)
	{
		SequencePlayer->Play();

		SequencePlayer->OnFinished.AddDynamic(this, &ACinematicTrigger::OnPlaybackFinished);
	}
}

void ACinematicTrigger::OnPlaybackFinished()
{
	if (ActiveCinematicTransform && ActiveCinematicTransform->bResetCamAfter)
	{
		if (GameMode)
		{
			GameMode->bUpdateCameraEveryTick = true;
		}
	}
}
