// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIPaintEater.h"
#include "GameFramework/Actor.h"
#include "Navigation/PathFollowingComponent.h"
#include "AIHouse.generated.h"

UCLASS()
class COLORLEAK_API AAIHouse : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAIHouse();

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxCollision;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;

	bool bIsActivated = false;
	UFUNCTION(BlueprintCallable)
	virtual void ChangeState();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlap(AActor* MyActor, AActor* OtherActor);

	void UpdateFlagColor(EColorType Color);
	void EndAnimation(AAIPaintEater *AI);
	FTimerHandle MoveTimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
