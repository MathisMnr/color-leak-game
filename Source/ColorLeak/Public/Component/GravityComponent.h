// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GravityComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COLORLEAK_API UGravityComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	FVector GravityDirection = FVector(0.f, 0.f, -1.f); // Default to normal gravity
	float GravityStrength = 1.0f; // Unreal Engine gravity value

public:
	UGravityComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InvertGravity();

	FVector GetGravityDirection() const { return GravityDirection; }
};
