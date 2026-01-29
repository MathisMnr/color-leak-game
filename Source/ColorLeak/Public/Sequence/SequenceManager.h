// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelSequenceActor.h"
#include "Tracks/MovieScene3DTransformTrack.h"
#include "LevelSequencePlayer.h"

UENUM()
enum class EKeyInterpolation : uint8
{
	Auto,
	Linear,
	Constant,
	Cubic
};

UENUM()
enum class EKeyframesDataSource : uint8
{
	Array,
	SourceLevelSequence
};


/**
 * 
 */
class COLORLEAK_API SequenceManager
{
public:
	SequenceManager();
	~SequenceManager();

	static ULevelSequence* CreateSequence(UMovieScene*& MovieScene, FFrameRate FrameRate, float SequenceLengthInSeconds);

	static void BindObjectToSequence(UMovieScene* MovieScene, FGuid& BindingID, ACameraActor* CameraActor, ULevelSequence* LevelSequence);

	static void AddTransformTrack(UMovieScene* MovieScene, FGuid BindingID, UMovieScene3DTransformSection*& TransformSection);

	static UMovieScene3DTransformSection* GetTrackFromLevelSequence(ULevelSequence* Sequence, AActor* Actor, int32 index = 0);

	static void AddKeyFrames(const UMovieScene* MovieScene, const UMovieScene3DTransformSection* TransformSection, FTransform Transform, double
					  TimeInSeconds);

	static void AddKeyFrameToChannel(FMovieSceneDoubleChannel* Channel, const FFrameNumber& FrameNumber, double Value, EKeyInterpolation KeyInterpolation);
};
