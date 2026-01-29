// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ColorTypes.h"
#include "GameFramework/Actor.h"
#include "Interface/PaintableInterface.h"
#include "ReflectiveBlock.generated.h"

UCLASS()
class COLORLEAK_API AReflectiveBlock : public AActor, public IPaintableInterface
{
	GENERATED_BODY()

public:
	AReflectiveBlock();

	EColorType GetColorType() const { return ColorType; }

	virtual void SetCurrentColor(EColorType newColor) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BlockMesh;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY(EditAnywhere)
	EColorType ColorType = EColorType::Uncolored;
};
