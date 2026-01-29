#pragma once

#include "CoreMinimal.h"
#include "ColoredPressurePlate.h"
#include "AI/AIHouse.h"
#include "GameFramework/Actor.h"
#include "Mechanism//Lever.h"
#include "Mechanism.generated.h"

UENUM(BlueprintType)
enum class EMainSolution : uint8
{
	Lever			UMETA(DisplayName = "Lever"),
	AIHouse			UMETA(DisplayName = "AI House"),
	PressurePlate	UMETA(DisplayName = "Pressure Plate"),
	Circles			UMETA(DisplayName = "Circles"),
	Laser			UMETA(DisplayName = "Laser")
};

UCLASS()
class COLORLEAK_API AMechanism : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMechanism();

	// The Lever linked to the mechanism
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mechanism Solution") ALever* LinkedLever;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mechanism Solution") AAIHouse* LinkedHouse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mechanism Solution") AColoredPressurePlate* LinkedPlate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mechanism Solution") int NbCircleNeeded;
	int NbCircleValidated;
	
	virtual void UpdateState() __override;
	UPROPERTY(EditAnywhere, Category = "Movement|Behavior") bool bIsActivated = true;
	UPROPERTY(EditAnywhere, Category = "Movement|Behavior") EMainSolution MainSolution = EMainSolution::Lever;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
