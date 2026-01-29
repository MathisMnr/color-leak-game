// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lever.generated.h"

UENUM(BlueprintType)
enum class ELeverGroupType : uint8
{
	OnlyOneActivated    UMETA(DisplayName = "Only One Activated"),
	OnlyOneDeactivated  UMETA(DisplayName = "Only One Deactivated")
};

UCLASS()
class ALever : public AActor
{
	GENERATED_BODY()
	
public:
	/*
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* SkeletalMeshComp;
	*/
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxCollision;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* SkeletalMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* LeverOffAnim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* LeverOnAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Sound")
	USoundBase* LeverActivationSound;

	// To read audio files
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
	class UAudioComponent* AudioComponent;
	
	// Sets default values for this actor's properties
	ALever();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, Category = "Animation")
	bool bIsActivated = true;
	UFUNCTION(BlueprintCallable)
	virtual void ChangeState(bool bIsEmiter = true);
	UFUNCTION(BlueprintCallable)
	bool GetLeverState();
	UPROPERTY(EditAnywhere, Category = "Lever Group")
	int GroupId;
	UPROPERTY(EditAnywhere, Category = "Lever Group")
	ELeverGroupType GroupType;
	void AdaptGroupState();
};
