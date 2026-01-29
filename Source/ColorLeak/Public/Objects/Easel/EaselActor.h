// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelSequencePlayer.h"
#include "GameFramework/Actor.h"
#include "Lobby/Character/LobbyCharacter.h"
#include "Tracks/MovieScene3DTransformTrack.h"
#include "EaselActor.generated.h"

enum class ELevels : uint8;

UCLASS()
class COLORLEAK_API AEaselActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* EaselMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* EaselWidgetComponent;

	UPROPERTY(EditAnywhere, Category = Widget)
	ELevels Level;

	UPROPERTY(EditAnywhere, Category = Widget)
	FSlateBrush ImageBrush;

	UPROPERTY(EditAnywhere, Category = Widget)
	bool bIsShowOnStart = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* InteractionWidget;

public:
	// Sets default values for this actor's properties
	AEaselActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlap(AActor* MyActor, AActor* OtherActor);

	UFUNCTION()
	void OnEndOverlap(AActor* MyActor, AActor* OtherActor);

public:
	void ShowWidget(bool bShow);

	bool IsShowWidget();

	void Interact();


	// The CameraActor that will be moved/rotated in the sequence
	UPROPERTY(EditAnywhere)
	ACameraActor* CameraActor;

	// camera component
	UPROPERTY(EditAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cinematic")
	FTransform EaselFinalTransform;

	UPROPERTY()
	ALobbyCharacter* LobbyCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cinematic")
	FFrameRate FrameRate = FFrameRate(60,1);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cinematic")
	double SequenceLengthInSeconds = 60;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Cinematic")
	ULevelSequence* LevelSequence = nullptr;

	// Play the Level Sequence
	void PlaySequence();

	UFUNCTION()
	void OnSequenceFinished();
};
