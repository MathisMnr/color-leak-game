// Fill out your copyright notice in the Description page of Project Settings.


#include "Sequence/SequenceManager.h"

#include "Camera/CameraActor.h"


SequenceManager::SequenceManager()
{
}

SequenceManager::~SequenceManager()
{
}

ULevelSequence* SequenceManager::CreateSequence(UMovieScene*& MovieScene, FFrameRate FrameRate, float SequenceLengthInSeconds)
{
    ULevelSequence* LevelSequence = NewObject<ULevelSequence>(GetTransientPackage(), ULevelSequence::StaticClass());
    LevelSequence->Initialize();

    MovieScene = LevelSequence->GetMovieScene();
    MovieScene->SetDisplayRate(FrameRate);

    const uint32 Duration = MovieScene->GetTickResolution().AsFrameNumber(SequenceLengthInSeconds).Value;
    MovieScene->SetPlaybackRange(FFrameNumber(0), Duration);

    return LevelSequence;
}

void SequenceManager::BindObjectToSequence(UMovieScene* MovieScene, FGuid& BindingID, ACameraActor* CameraActor, ULevelSequence* LevelSequence)
{
    BindingID = MovieScene->AddPossessable(CameraActor->GetName(), CameraActor->GetClass());
    LevelSequence->BindPossessableObject(BindingID, *CameraActor, CameraActor->GetWorld());
}

void SequenceManager::AddTransformTrack(UMovieScene* MovieScene, FGuid BindingID, UMovieScene3DTransformSection*& TransformSection)
{
    UMovieScene3DTransformTrack* TransformTrack = MovieScene->AddTrack<UMovieScene3DTransformTrack>(BindingID);
    TransformSection = Cast<UMovieScene3DTransformSection>(TransformTrack->CreateNewSection());
    TransformSection->SetRange(MovieScene->GetPlaybackRange());
    TransformTrack->AddSection(*TransformSection);
}

UMovieScene3DTransformSection* SequenceManager::GetTrackFromLevelSequence(ULevelSequence* Sequence, AActor* Actor, int32 index)
{
    UMovieScene3DTransformTrack* Track = Sequence->MovieScene->FindTrack<UMovieScene3DTransformTrack>(Sequence->FindBindingFromObject(Actor, Actor->GetWorld()));

    TArray<UMovieSceneSection*> AllSections = Track->GetAllSections();

    return Cast<UMovieScene3DTransformSection>(AllSections[index]);
}

void SequenceManager::AddKeyFrames(const UMovieScene* MovieScene, const UMovieScene3DTransformSection* TransformSection, FTransform Transform, double TimeInSeconds)
{
    const FMovieSceneChannelProxy& ChannelProxy = TransformSection->GetChannelProxy();

    FMovieSceneDoubleChannel* TranslationXChannel = ChannelProxy.GetChannel<FMovieSceneDoubleChannel>(0); // X
    FMovieSceneDoubleChannel* TranslationYChannel = ChannelProxy.GetChannel<FMovieSceneDoubleChannel>(1); // Y
    FMovieSceneDoubleChannel* TranslationZChannel = ChannelProxy.GetChannel<FMovieSceneDoubleChannel>(2); // Z

    FMovieSceneDoubleChannel* RotationXChannel = ChannelProxy.GetChannel<FMovieSceneDoubleChannel>(3); // Roll
    FMovieSceneDoubleChannel* RotationYChannel = ChannelProxy.GetChannel<FMovieSceneDoubleChannel>(4); // Pitch
    FMovieSceneDoubleChannel* RotationZChannel = ChannelProxy.GetChannel<FMovieSceneDoubleChannel>(5); // Yaw

    FMovieSceneDoubleChannel* ScaleXChannel = ChannelProxy.GetChannel<FMovieSceneDoubleChannel>(6); // Scale X
    FMovieSceneDoubleChannel* ScaleYChannel = ChannelProxy.GetChannel<FMovieSceneDoubleChannel>(7); // Scale Y
    FMovieSceneDoubleChannel* ScaleZChannel = ChannelProxy.GetChannel<FMovieSceneDoubleChannel>(8); // Scale Z

    EKeyInterpolation KeyInterpolation = EKeyInterpolation::Auto;

    const FVector Location = Transform.GetLocation();
    const FRotator Rotation = Transform.GetRotation().Rotator();
    const FVector Scale = Transform.GetScale3D();

    const FFrameNumber FrameNumber = MovieScene->GetTickResolution().AsFrameNumber(TimeInSeconds);

    AddKeyFrameToChannel(TranslationXChannel, FrameNumber, Location.X, KeyInterpolation);
    AddKeyFrameToChannel(TranslationYChannel, FrameNumber, Location.Y, KeyInterpolation);
    AddKeyFrameToChannel(TranslationZChannel, FrameNumber, Location.Z, KeyInterpolation);

    AddKeyFrameToChannel(RotationXChannel, FrameNumber, Rotation.Roll, KeyInterpolation);
    AddKeyFrameToChannel(RotationYChannel, FrameNumber, Rotation.Pitch, KeyInterpolation);
    AddKeyFrameToChannel(RotationZChannel, FrameNumber, Rotation.Yaw, KeyInterpolation);

    AddKeyFrameToChannel(ScaleXChannel, FrameNumber, Scale.X, KeyInterpolation);
    AddKeyFrameToChannel(ScaleYChannel, FrameNumber, Scale.Y, KeyInterpolation);
    AddKeyFrameToChannel(ScaleZChannel, FrameNumber, Scale.Z, KeyInterpolation);
}

void SequenceManager::AddKeyFrameToChannel(FMovieSceneDoubleChannel* Channel, const FFrameNumber& FrameNumber, double Value, EKeyInterpolation KeyInterpolation)
{
    switch (KeyInterpolation) {
    case EKeyInterpolation::Auto:
        AddKeyToChannel(Channel, FrameNumber, Value, EMovieSceneKeyInterpolation::Auto);
        break;
    case EKeyInterpolation::Linear:
        Channel->AddLinearKey(FrameNumber, Value);
        break;
    case EKeyInterpolation::Constant:
        Channel->AddConstantKey(FrameNumber, Value);
        break;
    case EKeyInterpolation::Cubic:
        Channel->AddCubicKey(FrameNumber, Value);
        break;
    }
}