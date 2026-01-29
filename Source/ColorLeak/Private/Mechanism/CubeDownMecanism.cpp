// Fill out your copyright notice in the Description page of Project Settings.


#include "Mechanism/CubeDownMecanism.h"
#include "Components/BoxComponent.h"
#include "Components/InterpToMovementComponent.h"

ACubeDownMecanism::ACubeDownMecanism() {

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	RootComponent = BoxCollider;

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform's Mesh"));
	PlatformMesh->SetupAttachment(RootComponent);

	MovementComponent = CreateDefaultSubobject<UInterpToMovementComponent>(TEXT("Movement Component"));

	MovementComponent->Duration = 5.0f;
	MovementComponent->bSweep = true;
	MovementComponent->BehaviourType = EInterpToBehaviourType::OneShot;
}

void ACubeDownMecanism::UpdateState() {

	Super::UpdateState();

	if (!bIsActivated) {

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Activation"));

		MovementComponent->ControlPoints.Add(FInterpControlPoint(FVector(0.f, 0.f, 0.f), true));
		for (int i = 0; i < Path.Num(); i++) {
			MovementComponent->ControlPoints.Add(FInterpControlPoint(Path[i], true));
		}
		MovementComponent->FinaliseControlPoints();

	}
	
}