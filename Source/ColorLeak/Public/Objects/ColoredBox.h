#pragma once

#include "CoreMinimal.h"
#include "ColorTypes.h" 
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../Interface/PaintableInterface.h"
#include "ColoredBox.generated.h"

UCLASS()
class AColoredBox : public AActor, public IPaintableInterface
{
	GENERATED_BODY()

public:
	AColoredBox();

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
	void OnOverlap(AActor* MyActor, AActor* OtherActor);
    
	UFUNCTION()
	void OnEndOverlap(AActor* MyActor, AActor* OtherActor);

public:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxCollision;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ArrowMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
	EColorType BoxColor;
	UFUNCTION(BlueprintCallable, Category = "Color")
	virtual void SetCurrentColor(EColorType newColor) override;
	bool IsSameColor(EColorType OtherColor);
	void UpdateMaterial();

	UPROPERTY(EditAnywhere, Category = "Linked Box") AColoredBox* LinkedBox;
	UPROPERTY(EditAnywhere, Category = "Linked Box") FVector BoxOffset;
	UPROPERTY(EditAnywhere, Category = "Linked Box") bool bIsMain;
	void UpdateLinkedBoxPosition();
	void UpdateLinkedBoxScale();
	FTimerHandle BoxSyncTimerHandle;
	FTimerHandle DelayedStopTimerHandle;

	void SetBoxMass(float NewMass);
	void InvertGravity(bool bIsGravityEnable, bool bIsOnLaunch);
};
