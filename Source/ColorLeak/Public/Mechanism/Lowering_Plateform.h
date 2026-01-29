#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Mechanism/Mechanism.h"
#include "Lowering_Plateform.generated.h"



class UCurveFloat;

UCLASS()
class COLORLEAK_API ALowering_Plateform : public AMechanism
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Arrow;

	UFUNCTION()
	void TimelineProgress(float value);


public:	
	// Sets default values for this actor's properties
	ALowering_Plateform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimeline CurveTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* CurveFloat;

	UPROPERTY()
	FVector StartLoc;
	UPROPERTY()
	FVector EndLoc;
	UPROPERTY(EditAnywhere, Category = "Timeline")
	float ZOffset;

	virtual void UpdateState() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnOverlap(AActor* MyActor, AActor* OtherActor);

	UFUNCTION()
	void OnEndOverlap(AActor* MyActor, AActor* OtherActor);

	int OnPlateform;

	FTimerHandle ReverseTimerHandle;
	UFUNCTION()
	void DelayedReverse();


};
