// Fill out your copyright notice in the Description page of Project Settings.


#include "Mechanism/Lever_Gravity.h"

#include "ColorLeak/ColorLeakCharacter.h"
#include "Component/GravityComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ALever_Gravity::ALever_Gravity()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALever_Gravity::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ALever_Gravity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALever_Gravity::ChangeState(bool bIsEmiter)
{
	Super::ChangeState(bIsEmiter);

	bIsGravityInverted = !bIsGravityInverted;

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Gravity"), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		UGravityComponent* GravityComp = Actor->FindComponentByClass<UGravityComponent>();
		if (GravityComp)
		{
			GravityComp->InvertGravity();
		}
	}

	FoundActors.Empty();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AColorLeakCharacter::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		AColorLeakCharacter* Character = Cast<AColorLeakCharacter>(Actor);
		if (Character)
		{
			Character->GetCharacterMovement()->SetGravityDirection(FVector(0.f, 0.f, bIsGravityInverted ? 1.f : -1.f));
		}
	}
}

