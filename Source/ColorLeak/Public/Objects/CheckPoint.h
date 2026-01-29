// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ColorTypes.h"
#include "NiagaraSystem.h"
#include "GameFramework/Actor.h"
#include "../ColorLeakCharacter.h"
#include "CheckPoint.generated.h"

UENUM(BlueprintType)
enum class ECheckPointType : uint8
{
	Both	UMETA(DisplayName = "Both"),
	Single	UMETA(DisplayName = "Single")
};

UCLASS()
class COLORLEAK_API ACheckPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckPoint();

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;
	
	EColorType CheckPointColor;
	void changeColor(EColorType NewColor);

	UPROPERTY(EditAnywhere)
	ECheckPointType CheckPointType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* CheckPointSound;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	UNiagaraComponent* NiagaraSystemCheckPoint;

	bool bIsActive;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnOverlap(AActor* MyActor, AActor* OtherActor);
	void UpdateRespawnLocation(AColorLeakCharacter* Character, const FVector& NewLocation);
};
