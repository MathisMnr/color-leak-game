// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PaintableInterface.h"
#include "Cannon.generated.h"

UCLASS()
class COLORLEAK_API ACannon : public AActor, public IPaintableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACannon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Collision")
	class UBoxComponent* BoxCollision;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ColorIndicatorStone; // The color Indicator

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* CanonShotSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* InvalidColorSound;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* LitFuseSound;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* ChargingBulletSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
	EColorType Color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activation")
	FString ActivationColor = "None";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activation")
	bool bUsable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activation")
	bool bIsAttachedToPlatform = false;

	UFUNCTION(BlueprintImplementableEvent, Category="BP")
	void BPPlaySFX();

	UFUNCTION(BlueprintImplementableEvent, Category="BP")
	void BPShot();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void UpdateMaterial();

	// Prend en paramètre un material
	void IsCorrectlyColored(FString Color, UMaterialInterface* OldMaterial);

	// Shot function
	void Shot(UMaterialInterface* OldMaterial);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetCurrentColor(EColorType newColor) override;
};




