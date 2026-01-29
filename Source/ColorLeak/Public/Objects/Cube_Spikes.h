// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cube_Spikes.generated.h"

UCLASS()
class COLORLEAK_API ACube_Spikes : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
    class UBoxComponent* BoxCollision;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent* StaticMesh;

public:	
	// Sets default values for this actor's properties
	ACube_Spikes();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnOverlap(AActor* MyActor, AActor* OtherActor);
};
