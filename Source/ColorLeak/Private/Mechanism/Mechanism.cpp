#include "Mechanism/Mechanism.h"

// Sets default values
AMechanism::AMechanism()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	NbCircleValidated = 0;
	NbCircleNeeded = 0;
}

// Called when the game starts or when spawned
void AMechanism::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMechanism::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMechanism::UpdateState()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("AMechanism::UpdateState"));
	if (LinkedLever && MainSolution == EMainSolution::Lever)
	{
		bIsActivated = LinkedLever->bIsActivated;
		// FString State = bIsActivated ? "Activated" : "Deactivated";
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Mechanism is %s"), *State));
	}
	if (LinkedHouse && MainSolution == EMainSolution::AIHouse)
	{
		bIsActivated = LinkedHouse->bIsActivated;
		//FString State = bIsActivated ? "Activated" : "Deactivated";
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Linked house is %d"), LinkedHouse->bIsActivated));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Mechanism is %s"), *State));
	}
	if (LinkedPlate && MainSolution == EMainSolution::PressurePlate)
	{
		bIsActivated = LinkedPlate->bIsActivated;
	}
	if (MainSolution == EMainSolution::Circles)
	{
		NbCircleValidated++;
		if (NbCircleValidated == NbCircleNeeded) bIsActivated = !bIsActivated;
	}
	if (MainSolution == EMainSolution::Laser)
	{
		bIsActivated = !bIsActivated;
	}
}
