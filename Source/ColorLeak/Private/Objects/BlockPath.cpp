// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/BlockPath.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ABlockPath::ABlockPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	MovementComponent = CreateDefaultSubobject<UInterpToMovementComponent>(TEXT("Movement Component"));

	MovementComponent->Duration = 5.0f;
	MovementComponent->bSweep = true;
}

// Called when the game starts or when spawned
void ABlockPath::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABlockPath::OpenPath(){

	nbCircleValidate++;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Nb circle : %d"), nbCircleValidate));

	if(nbCircleTotal == nbCircleValidate){
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Path Open !"));

		if(WallDownSound){
			UGameplayStatics::SpawnSoundAttached(WallDownSound, StaticMesh);
		}

		MovementComponent->ControlPoints.Add(FInterpControlPoint(FVector(0,0,0), true));
		for (int i = 0; i < Path.Num(); i++) {
			MovementComponent->ControlPoints.Add(FInterpControlPoint(Path[i], true));
		}
		MovementComponent->FinaliseControlPoints();
	}
}

// Called every frame
void ABlockPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

