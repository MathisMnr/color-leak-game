#pragma once

#include "CoreMinimal.h"
#include "ColorTypes.h"
#include "GameFramework/Actor.h"
#include "ColoredPressurePlate.generated.h"


UCLASS()
class AColoredPressurePlate : public AActor
{
	GENERATED_BODY()

public:
	AColoredPressurePlate();

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxCollision;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Sound") USoundBase* PlateActivationSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Sound") USoundBase* PlateDeactivationSound;
	// To read audio files
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
	class UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mechanism") bool bIsActivated = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mechanism") EColorType PlateColor;
	void UpdateColor(EColorType NewColor);

protected:
	virtual void BeginPlay() override;

private :
	UFUNCTION()
	void OnOverlap(AActor* MyActor, AActor* OtherActor);

	UFUNCTION()
	void OnEndOverlap(AActor* MyActor, AActor* OtherActor);

public:
	virtual void Tick(float DeltaTime) override;

	
	UFUNCTION(BlueprintCallable)
	virtual void ChangeState(bool bIsEmiter = true);
	UFUNCTION(BlueprintCallable)
	bool GetPlateState();

	bool IsValidActor(AActor* OtherActor);

	// Animation & audio
	void StartPlateAnimation(bool bReverse);
	void UpdatePlateScale();
	FTimerHandle PlateMoveTimerHandle;
	FVector InitialScale;
	FVector TargetScale;
	float AnimationDuration = 0.2f; 
	float AnimationStepTime = 0.01f;
	float ElapsedTime = 0.0f;
	bool bIsGoingDown = false;

	int32 OverlappingValidActors = 0;
};
