// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/UnlinkZone.h"

#include "Components/BoxComponent.h"
#include "Objects/ColoredBox.h"

// Sets default values
AUnlinkZone::AUnlinkZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	RootComponent = BoxCollision;
}

// Called when the game starts or when spawned
void AUnlinkZone::BeginPlay()
{
	Super::BeginPlay();

	// Bind function OnActorBeginOverlap with your class function OnOverlap
	this->OnActorBeginOverlap.AddDynamic(this, &AUnlinkZone::OnOverlap);

}

void AUnlinkZone::OnOverlap(AActor* MyActor, AActor* OtherActor)
{
	if (auto ColoredBox = Cast<::AColoredBox>(OtherActor))
	{
		if (!bIsActive)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Unlink")) );
			ColoredBox->LinkedBox->LinkedBox = nullptr;
			ColoredBox->SetActorLocation(GetActorLocation());
		}
		bIsActive = true;
	}
}
