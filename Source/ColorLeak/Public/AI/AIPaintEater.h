// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ColorTypes.h"
#include "Acryl.h"
#include "Blueprint/UserWidget.h"
#include "AIPaintEater.generated.h"

UCLASS()
class COLORLEAK_API AAIPaintEater : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIPaintEater();

	UPROPERTY(EditAnywhere)
	EColorType Color;
	void UpdateColor();
	
	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* TreeAsset;
	UPROPERTY(EditAnywhere, Category = "AI")
	class UBlackboardData* BlackboardAsset;
	bool bIsChasing;
	void SetIsChasing(bool bNewState);
	void SetGravity(bool bNewGravityState);

	UPROPERTY(EditAnywhere)
	float DetectionDistance;
	UPROPERTY(EditAnywhere)
	float EatingRange;
	UPROPERTY(EditAnywhere)
	float EatingTime = 3.0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class UBoxComponent* EatingDetectionBox;
	
	UPROPERTY()
	UUserWidget* ProgressBarWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> ProgressBarWidgetClass;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Jump, meta = (AllowPrivateAccess = "true"))
	bool bIsEating;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio") class UAudioComponent* AudioComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Sound") USoundWave* LickSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Sound") USoundWave* BlurpSound;

	FTimerHandle LickSoundTimerHandle;
	int LastPlayedLickSoundTime = -1;

	bool bIsLaunched = false;
	FTimerHandle WigdetPositionTimerHandle;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:
	UFUNCTION()
	void OnOverlap(AActor* MyActor, AActor* OtherActor);
	UFUNCTION()
	void OnEndOverlap(AActor* MyActor, AActor* OtherActor);
	
	FTimerHandle EatingTimerHandle; // Pour gérer le temps d'alimentation
 // Pour savoir si l'IA est en train de manger la couleur

	float EatingTimeCounter;
	void UpdateProgressBar(AAcryl* Acryl);
	void UpdateWidgetPosition();
	void UpdateProgressBarColor();
	
};
