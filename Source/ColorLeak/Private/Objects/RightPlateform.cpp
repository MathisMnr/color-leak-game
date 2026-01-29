// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/RightPlateform.h"
#include "Components/BoxComponent.h"
#include "Components/InterpToMovementComponent.h"
#include "Chromarion.h"
#include "Acryl.h"
#include "DSP/DelayStereo.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ARightPlateform::ARightPlateform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	RootComponent = BoxCollider;

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform's Mesh"));
	PlatformMesh->SetupAttachment(RootComponent);

	MovementComponent = CreateDefaultSubobject<UInterpToMovementComponent>(TEXT("Movement Component"));

	MovementComponent->Duration = 5.0f;
	MovementComponent->bSweep = true;
	
	MovementComponent->SetAutoActivate(true);
	stateMovement = false;
	bIsRelative = true;
	bNeedBothCharacter = false;
	bIsOverlap = true;

	bChromarionOnPlatform = false;
	bAcrylOnPlatform = false;
}

// Called when the game starts or when spawned
void ARightPlateform::BeginPlay()
{
	Super::BeginPlay();

	// Bind function OnActorBeginOverlap with your class function OnOverlap
	this->OnActorBeginOverlap.AddDynamic(this, &ARightPlateform::OnOverlap);

	// Bind function OnActorBeginOverlap with your class function EndOverlap
	this->OnActorEndOverlap.AddDynamic(this, &ARightPlateform::OnEndOverlap);
	
	
	StartMovement();
	if (!bIsActivated)
	{
		MovementComponent->SetComponentTickEnabled(false);
		MovementComponent->SetAutoActivate(false);
	}
	else
	{
		MovementComponent->SetComponentTickEnabled(true);
		MovementComponent->SetAutoActivate(true);
	}
}

void ARightPlateform::OnOverlap(AActor* MyActor, AActor* OtherActor)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("bIsActivated: %d "),bIsActivated) );
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("bNeedBothCharacter: %d "),bNeedBothCharacter) );
	if (bIsOverlap)
	{
		if (auto c = Cast<AChromarion>(OtherActor))
		{
			bChromarionOnPlatform = true;
		}
		else if (auto a = Cast<AAcryl>(OtherActor))
		{
			a->GetCharacterMovement()->SetBase(nullptr);
			a->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			a->GetCharacterMovement()->bIgnoreBaseRotation = false;
			bAcrylOnPlatform = true;
		}
	
		if (OtherActor && !bIsActivated && !bNeedBothCharacter)					// if we step on it, it activates -- no deactivation if we leave platform
		{	
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Collision"));
			bIsActivated = !bIsActivated;
			MovementComponent->SetComponentTickEnabled(true);
		}

		if (bAcrylOnPlatform && bChromarionOnPlatform && !bIsActivated && bNeedBothCharacter)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Collision"));
			bIsActivated = !bIsActivated;
			MovementComponent->SetComponentTickEnabled(true);
		}
	}
}

void ARightPlateform::OnEndOverlap(AActor* MyActor, AActor* OtherActor)
{
	if (Cast<AChromarion>(OtherActor))
	{
		bChromarionOnPlatform = false;
	}
	else if (Cast<AAcryl>(OtherActor))
	{
		bAcrylOnPlatform = false;
	}
}

void ARightPlateform::StartMovement()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("StartMovement"));
	MovementComponent->ControlPoints.Add(FInterpControlPoint(FVector(0,0,0), true));
	for (int i = 0; i < Path.Num(); i++) {
		MovementComponent->ControlPoints.Add(FInterpControlPoint(Path[i], bIsRelative));
	}
	MovementComponent->FinaliseControlPoints();
}


void ARightPlateform::UpdateState()
{
	AMechanism::UpdateState();
	if (this->MovementComponent->BehaviourType == EInterpToBehaviourType::PingPong)
	{
		if (bIsActivated)
		{
			//MovementComponent->SetComponentTickEnabled(true);
			MovementComponent->SetComponentTickEnabled(stateMovement);
		}
		else
		{
			MovementComponent->SetComponentTickEnabled(!stateMovement);
		}
	}
	
	if (this->MovementComponent->BehaviourType == EInterpToBehaviourType::OneShot)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("OneShot"));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("stateMovement is %d"), stateMovement));
		MovementComponent->SetComponentTickEnabled(bIsActivated);
	}
	
	if (this->MovementComponent->BehaviourType == EInterpToBehaviourType::OneShot_Reverse)
	{
		if (bIsActivated)
		{
			//MovementComponent->SetComponentTickEnabled(true);
			MovementComponent->SetComponentTickEnabled(stateMovement);
		}
		else
		{
			MovementComponent->SetComponentTickEnabled(!stateMovement);
		}
	}
}

// Called every frame
void ARightPlateform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


