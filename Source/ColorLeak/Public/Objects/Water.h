#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ColorTypes.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Chromarion.h"
#include "Acryl.h"
#include "Objects/ColoredBox.h"

#include "Water.generated.h"

UCLASS()
class COLORLEAK_API AWater : public AActor
{
	GENERATED_BODY()
    
public:    
	AWater();

	UFUNCTION()
	void SetFallingSpeed(float Speed);

	UPROPERTY(EditAnywhere, Category = "Water Properties")
	bool bIsDynamic;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void MoveDown(); 

	UFUNCTION()
	void OnOverlap(AActor* MyActor, AActor* OtherActor);

	UPROPERTY(EditAnywhere, Category = "Water Properties")
	class UBoxComponent* BoxCollision;
    
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
	class UAudioComponent* AudioComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Sound")
	USoundBase* SplashSound;

private:
	float FallingSpeed; 
	float MoveInterval; 
	FTimerHandle MoveTimer;
};
