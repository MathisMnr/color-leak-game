// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lever.h"
#include "Lever_Gravity.generated.h"

UCLASS()
class COLORLEAK_API ALever_Gravity : public ALever
{
	GENERATED_BODY()

	bool bIsGravityInverted = false;

public:
	// Sets default values for this actor's properties
	ALever_Gravity();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ChangeState(bool bIsEmiter = true) override;
};
