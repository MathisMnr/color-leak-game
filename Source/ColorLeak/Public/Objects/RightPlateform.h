// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mechanism/Mechanism.h"
#include "RightPlateform.generated.h"

UCLASS()
class COLORLEAK_API ARightPlateform : public AMechanism
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARightPlateform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path", meta = (ExposeOnSpawn = true, MakeEditWidget = true))
	TArray<FVector> Path;
	virtual void UpdateState() override;
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true")) class UBoxComponent* BoxCollider;
	UPROPERTY(EditDefaultsOnly, Category = "Components") class UStaticMeshComponent* PlatformMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Components") class UInterpToMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere, Category = "Movement|Behavior") bool bIsRelative;
	UPROPERTY(EditAnywhere, Category = "Movement|Behavior") bool stateMovement;
	UPROPERTY(EditAnywhere, Category = "Movement|Behavior") bool bNeedBothCharacter;
	UPROPERTY(EditAnywhere, Category = "Movement|Behavior") bool bIsOverlap;
	
	bool bChromarionOnPlatform;
	bool bAcrylOnPlatform;

	void StartMovement();
	
	UFUNCTION()
	void OnOverlap(AActor* MyActor, AActor* OtherActor);

	UFUNCTION()
	void OnEndOverlap(AActor* MyActor, AActor* OtherActor);
};
