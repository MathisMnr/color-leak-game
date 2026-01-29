// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnlinkZone.generated.h"

UCLASS()
class COLORLEAK_API AUnlinkZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUnlinkZone();

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxCollision;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private :
	UFUNCTION()
	void OnOverlap(AActor* MyActor, AActor* OtherActor);
	bool bIsActive = false;
	
};
