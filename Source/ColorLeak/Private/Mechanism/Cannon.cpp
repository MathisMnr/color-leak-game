// Fill out your copyright notice in the Description page of Project Settings.


#include "Mechanism/Cannon.h"

#include "Acryl.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimSequence.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ACannon::ACannon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Ajout du tag "Paintable" à l'acteur, visible en blueprint
	Tags.Add(FName("Paintable"));

	// Création du BoxCollision
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;
	BoxCollision->SetBoxExtent(FVector(100.0f, 100.0f, 50.0f));
	BoxCollision->SetCollisionProfileName("OverlapAllDynamic");

	BoxCollision->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	BoxCollision->SetCollisionObjectType(ECC_PhysicsBody);
	BoxCollision->SetSimulatePhysics(true);

	// Création du StaticMesh
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	// Création du ColorIndicatorStone
	ColorIndicatorStone = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ColorIndicatorStone"));
	ColorIndicatorStone->SetupAttachment(RootComponent);

	////////////////////////////////////////////

	// Chargement du Static Mesh (Le canon)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Assets/Decors/Level_Mechanics/Cannon.Cannon"));
	if (MeshAsset.Succeeded())
	{
		StaticMesh->SetStaticMesh(MeshAsset.Object);
		UE_LOG(LogTemp, Log, TEXT("Loaded StaticMesh for Canon"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load StaticMesh for Canon"));
	}

	// Rotation du StaticMesh
	StaticMesh->SetRelativeRotation(FRotator(0, 0, 0));

	// Chargement du ColorIndicatorStone
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ColorIndicatorStoneAsset(TEXT("/Game/Assets/Decors/NatureKit/statue_block.statue_block"));
	if (ColorIndicatorStoneAsset.Succeeded())
	{
		ColorIndicatorStone->SetStaticMesh(ColorIndicatorStoneAsset.Object);
		// Attache ColorIndicatorStone au StaticMesh (et non plus au RootComponent)
		ColorIndicatorStone->SetupAttachment(StaticMesh);
		UE_LOG(LogTemp, Log, TEXT("Loaded StaticMesh for Color Indicator Stone"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load StaticMesh for Color Indicator Stone"));
	}

	// Déplacement et redimensionnement de la BoxCollision
	BoxCollision->SetRelativeLocation(FVector(0, 0, 50));
	BoxCollision->SetWorldScale3D(FVector(1, 1, 1));

	// Chargement du Son de tir
	static ConstructorHelpers::FObjectFinder<USoundBase> CanonShotAsset(TEXT("/Game/Assets/Decors/Level_Mechanics/SFX/Canon_Shot_Sound_Effect"));
	if (CanonShotAsset.Succeeded())
	{
		CanonShotSound = CanonShotAsset.Object;
		UE_LOG(LogTemp, Log, TEXT("Loaded Sound for Canon Shot"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load Sound for Canon Shot"));
		GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, TEXT("Not loaded Canon Shot Sound"));
	}

	// Chargement du Son de couleur invalide
	static ConstructorHelpers::FObjectFinder<USoundBase> InvalidColorSoundAsset(TEXT("/Game/Assets/Decors/Level_Mechanics/SFX/Incorrect_Sound_Effect"));
	if (InvalidColorSoundAsset.Succeeded())
	{
		InvalidColorSound = InvalidColorSoundAsset.Object;
		UE_LOG(LogTemp, Log, TEXT("Loaded Sound for Invalid Color"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load Sound for Invalid Color"));
		GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, TEXT("Not loaded Invalid Color Sound"));
	}

	// Chargement du son de mèche allumée
	static ConstructorHelpers::FObjectFinder<USoundBase> LitFuseSoundAsset(TEXT("/Game/Assets/Decors/Level_Mechanics/SFX/Lit_Fuse_Sound_Effect-_AudioTrimmer_com_.Lit_Fuse_Sound_Effect-_AudioTrimmer_com_"));
	if (LitFuseSoundAsset.Succeeded())
	{
		LitFuseSound = LitFuseSoundAsset.Object;
		UE_LOG(LogTemp, Log, TEXT("Loaded Sound for Lit Fuse"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load Sound for Lit Fuse"));
        GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, TEXT("Not loaded Lit Fuse Sound"));
	}

	// Chargement du son de balle chargée
	static ConstructorHelpers::FObjectFinder<USoundBase> ChargingBulletSoundAsset(TEXT("/Game/Assets/Decors/Level_Mechanics/SFX/Charging_Bullet_Sound_Effect.Charging_Bullet_Sound_Effect"));
	if (ChargingBulletSoundAsset.Succeeded())
    {
        ChargingBulletSound = ChargingBulletSoundAsset.Object;
        UE_LOG(LogTemp, Log, TEXT("Loaded Sound for Charging Bullet"));
    }
    else
    {
	    UE_LOG(LogTemp, Error, TEXT("Failed to load Sound for Charging Bullet"));
    	GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, TEXT("Not loaded Charging Bullet Sound"));
    }
	
	// Création du composant de particules de la mèche allumée
	//ParticleSystemComponentLitFuse = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));

	// Création du composant de particules de la détonation
	//ParticleSystemComponentExplosion = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponentExplosion"));

	//ParticleSystemComponentLitFuse->AttachToComponent(StaticMesh, FAttachmentTransformRules::KeepRelativeTransform);
	//ParticleSystemComponentExplosion->AttachToComponent(StaticMesh, FAttachmentTransformRules::KeepRelativeTransform);

	// Désactiver l'activation automatique
	//ParticleSystemComponentLitFuse->bAutoActivate = false;
	//ParticleSystemComponentExplosion->bAutoActivate = false;
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();

	// Application de la ActivationColor pour la ColorIndicatorStone
	FString MaterialPathActivationColor = FString::Printf(TEXT("Material'/Game/Characters/Chromarion/Materials/%s/M_Secondary.M_Secondary'"), *ActivationColor);
	UMaterialInterface* ActivationColorMaterial = LoadObject<UMaterialInterface>(nullptr, *MaterialPathActivationColor);

	if (ActivationColorMaterial)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Great"));
		ColorIndicatorStone->SetMaterial(1, ActivationColorMaterial);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load Material for Color Indicator Stone"));
	}

	// On set Usable à true
	this->bUsable = true;
}

// Called every frame
void ACannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACannon::SetCurrentColor(EColorType newColor)
{
	IPaintableInterface::SetCurrentColor(newColor);

	Color = newColor;
	UpdateMaterial();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Canon"));
}

void ACannon::UpdateMaterial()
{
	FString ColorName;

	switch (Color)
	{
	case EColorType::Red:
		ColorName = TEXT("Red");
		break;
	case EColorType::Yellow:
		ColorName = TEXT("Yellow");
		break;
	case EColorType::Blue:
		ColorName = TEXT("Blue");
		break;
	case EColorType::Green:
		ColorName = TEXT("Green");
		break;
	case EColorType::Orange:
		ColorName = TEXT("Orange");
		break;
	case EColorType::Purple:
		ColorName = TEXT("Purple");
		break;
	case EColorType::Uncolored:
		ColorName = TEXT("UnColored");
		break;
	default:
		ColorName = TEXT("UnColored");
		break;
	}

	// On instancie l'ancien materiel pour le remettre après activation du canon
	UMaterialInterface* OldMaterial = StaticMesh->GetMaterial(1);

	FString SecondaryMaterialPath = FString::Printf(TEXT("Material'/Game/Characters/Chromarion/Materials/%s/M_Secondary.M_Secondary'"), *ColorName);
	UMaterialInterface* CanonColor = LoadObject<UMaterialInterface>(nullptr, *SecondaryMaterialPath);
	
	if (CanonColor)
	{
		// Apply material to Cannon
		StaticMesh->SetMaterial(1, CanonColor);
		IsCorrectlyColored(ColorName, OldMaterial);
	}
	
}

void ACannon::IsCorrectlyColored(FString NewColor, UMaterialInterface* OldMaterial)
{
	if (NewColor == ActivationColor && bUsable)
	{
		this->bUsable = false;
		Shot(OldMaterial);
	}
	else if (NewColor != ActivationColor && bUsable)
	{
		this->bUsable = false;
		UGameplayStatics::PlaySoundAtLocation(this, InvalidColorSound, GetActorLocation());
		// Attendre 2 secondes avant de reset le material
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, OldMaterial]() {
			StaticMesh->SetMaterial(1, OldMaterial);
		}, 2.0f, false);
	}
}

void ACannon::Shot(UMaterialInterface* OldMaterial)
{
	// Activer les particules
	//ParticleSystemComponentLitFuse->ActivateSystem();
	FTimerHandle ParticuleTimerHandle;
	UGameplayStatics::PlaySoundAtLocation(this, LitFuseSound, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(this, ChargingBulletSound, GetActorLocation());
	BPPlaySFX();
	
	// Lancer un timer pour désactiver les particules après 2 secondes
	GetWorld()->GetTimerManager().SetTimer(ParticuleTimerHandle, [this]
	{
		UGameplayStatics::PlaySoundAtLocation(this, CanonShotSound, GetActorLocation());
		BPShot();
	}, 2.0f, false);

	FTimerHandle ResetTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(ResetTimerHandle, [this, OldMaterial]() {
		StaticMesh->SetMaterial(1, OldMaterial);
		this->bUsable = true;
	}, 4.0f, false);
	
}


