// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Cube_Spikes.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../../ColorLeakCharacter.h"

// Sets default values
ACube_Spikes::ACube_Spikes()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    RootComponent = StaticMesh;

    BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
    BoxCollision->SetupAttachment(StaticMesh);

}

// Called when the game starts or when spawned
void ACube_Spikes::BeginPlay()
{
	Super::BeginPlay();

    this->OnActorBeginOverlap.AddDynamic(this, &ACube_Spikes::OnOverlap);
	
}

// Called every frame
void ACube_Spikes::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACube_Spikes::OnOverlap(AActor* MyActor, AActor* OtherActor)
{
    if (auto Player = Cast<AColorLeakCharacter>(OtherActor))
    {
        Player->DeathCharacter();
        //UKismetSystemLibrary::PrintString(this, TEXT("Dead"), true, true, FLinearColor::Red, 2.0f);
    }
}

