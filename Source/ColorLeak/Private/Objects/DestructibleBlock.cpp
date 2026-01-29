// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/DestructibleBlock.h"

#include "Chromarion.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ADestructibleBlock::ADestructibleBlock()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Création du BoxCollision
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;
	BoxCollision->SetBoxExtent(FVector(100.0f, 100.0f, 50.0f));
	BoxCollision->SetCollisionProfileName("OverlapAllDynamic");
	

	// Création du StaticMesh
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Assets/Decors/Cubes/Cube_Bricks.Cube_Bricks"));
	if (MeshAsset.Succeeded())
	{
		StaticMesh->SetStaticMesh(MeshAsset.Object);
		UE_LOG(LogTemp, Log, TEXT("Loaded StaticMesh for Canon"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load StaticMesh for Canon"));
	}
	
	StaticMesh->SetRelativeRotation(FRotator(0, 0, 0));

	static ConstructorHelpers::FObjectFinder<USoundBase> BlockDestroySoundAsset(TEXT("/Game/Assets/Decors/Level_Mechanics/SFX/Block_Breaking_Sound_Effect.Block_Breaking_Sound_Effect"));
	if (BlockDestroySoundAsset.Succeeded())
	{
		BlockDestroySound = BlockDestroySoundAsset.Object;
		UE_LOG(LogTemp, Log, TEXT("Loaded Sound for Block Destroying"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load Sound for Canon Shot"));
		GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, TEXT("Not loaded Block Destroying Sound"));
	}

	// Création du composant de particules de la destruction de l'objet
	ParticleSystemComponentDestruction = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));

	// On l'attache au static mesh
	ParticleSystemComponentDestruction->SetupAttachment(StaticMesh);
}

// Called when the game starts or when spawned
void ADestructibleBlock::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADestructibleBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADestructibleBlock::DestroyByJumping(AChromarion* Chromarion)
{
	// On vérifie si la couleur de Chromarion est rouge
	if (Chromarion->GetCurrentColor() == EColorType::Red && Chromarion->bIsPowerActive)
	{
		// Spawn de la particule
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystemComponentDestruction->Template, GetActorLocation());
		// On joue le son de destruction
		UGameplayStatics::PlaySoundAtLocation(this, BlockDestroySound, GetActorLocation());
		Destroy();
	}
}




