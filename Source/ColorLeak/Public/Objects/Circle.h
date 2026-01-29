// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RightPlateform.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "../Interface/PaintableInterface.h"
#include "Circle.generated.h"

class ABlockPath;

UCLASS()
class COLORLEAK_API ACircle : public AActor, public IPaintableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACircle();

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxCollision;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mechanism")
	ABlockPath* BlockPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mechanism")
	AMechanism* Mechanism;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* ValidateSound;
	
	UPROPERTY(EditAnywhere, Category = "States") 
	bool bIsActive = true;

	UPROPERTY(EditAnywhere, Category = "States") 
	bool bIsValidate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
	EColorType CircleColor;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Color")
	virtual void SetCurrentColor(EColorType newColor) override;

	void UpdateMaterial();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnOverlap(AActor* MyActor, AActor* OtherActor);
    
	UFUNCTION()
	void OnEndOverlap(AActor* MyActor, AActor* OtherActor);

};
