// Fill out your copyright notice in the Description page of Project Settings.


#include "Laser/Receiver/ChangeColorBlock.h"


// Sets default values
AChangeColorBlock::AChangeColorBlock()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AChangeColorBlock::BeginPlay()
{
	Super::BeginPlay();

	DynamicMaterial = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
}

// Called every frame
void AChangeColorBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChangeColorBlock::ActivateMechanism(EColorType Color)
{
	Super::ActivateMechanism(Color);

	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue("BaseColor", FromColor(Color));
	}
}
