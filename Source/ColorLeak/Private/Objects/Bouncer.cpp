// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Bouncer.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Animation/AnimSequence.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Default value for the bounce force
//const float BounceForce = 200.0f;


// Sets default values
ABouncer::ABouncer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Création du BoxCollision
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;
	BoxCollision->SetBoxExtent(FVector(100.0f, 100.0f, 50.0f));
	BoxCollision->SetCollisionProfileName("OverlapAllDynamic");

	// Création du SkeletalMesh
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);

	// Création de l'AudioComponent
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->bAutoActivate = false; // Ne joue pas le son automatiquement

	// Bind de la fonction d'Overlap
	//BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ABouncer::OnOverlapBegin);

	////////////////////////////////////////////

	// Chargement du Skeletal Mesh
	if (SkeletalMesh)
	{
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Assets/Decors/Level_Mechanics/Bouncer.Bouncer"));
		if (MeshAsset.Succeeded())
		{
			SkeletalMesh->SetSkeletalMesh(MeshAsset.Object);
			UE_LOG(LogTemp, Warning, TEXT("Loaded SkeletalMesh for Bouncer"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load SkeletalMesh for Bouncer"));
		}
		// Rotation du SkeletalMesh
		SkeletalMesh->SetRelativeRotation(FRotator(0, 0, 0));
	}

	// Déplacement et redimensionnement de la BoxCollision
	if (BoxCollision)
	{
		BoxCollision->SetRelativeLocation(FVector(0, 0, 50));
		BoxCollision->SetWorldScale3D(FVector(2.5, 2.5, 2.5));
	}

	// Chargement de l'Animation de rebond
	static ConstructorHelpers::FObjectFinder<UAnimSequence> JumpAnimAsset(TEXT("/Game/Assets/Decors/Level_Mechanics/Bouncer_Anim_Bouncer_Bounce.Bouncer_Anim_Bouncer_Bounce"));
	if (JumpAnimAsset.Succeeded())
	{
		SpingJumpAnim = JumpAnimAsset.Object;
		UE_LOG(LogTemp, Warning, TEXT("Loaded Animation for Bouncer Jump"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load Animation for Bouncer Jump"));
	}

	// Chargement du Son de rebond
	static ConstructorHelpers::FObjectFinder<USoundBase> JumpSoundAsset(TEXT("/Game/Assets/Decors/Level_Mechanics/SFX/Spring_Bounces.Spring_Bounces"));
	if (JumpSoundAsset.Succeeded())
	{
		SpringJumpSound = JumpSoundAsset.Object;
		UE_LOG(LogTemp, Warning, TEXT("Loaded Sound for Bouncer Jump"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load Sound for Bouncer Jump"));
		GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, "Not loaded");
	}

	// Application du son à l'AudioComponent
	if (AudioComponent)
	{
		AudioComponent->SetSound(SpringJumpSound);
	}
}

// Called when the game starts or when spawned
void ABouncer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABouncer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABouncer::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Si l'acteur qui entre en collision est un Character
	if (OtherActor)
	{
		GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, "Bouncer");
		// Cast pour vérifier si c'est un personnage
		ACharacter* Character = Cast<ACharacter>(OtherActor);
		if (Character)
		{
			// Accès au CharacterMovement pour remplacer la vitesse verticale
			UCharacterMovementComponent* CharMove = Character->GetCharacterMovement();
			if (CharMove)
			{
				FVector NewVelocity = CharMove->Velocity;
				NewVelocity.Z = 500;
				CharMove->Velocity = NewVelocity;
			}

			// Jouer l'animation du ressort
			if (SpingJumpAnim)
			{
				SkeletalMesh->PlayAnimation(SpingJumpAnim, false);
			}

			// Jouer le son du ressort
			if (SpringJumpSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, SpringJumpSound, GetActorLocation());
			}
		}
	}
}