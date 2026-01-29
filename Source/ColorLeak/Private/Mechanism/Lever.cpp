// Fill out your copyright notice in the Description page of Project Settings.

#include "Mechanism//Lever.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequence.h"
#include "EngineUtils.h"
#include "Mechanism/Mechanism.h"
#include "Components/AudioComponent.h"
#include "Engine/LevelScriptActor.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/OutputDeviceNull.h"

// Sets default values
ALever::ALever()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create StaticMeshComponent and Attach to BoxComponent
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = SkeletalMesh;

	// Create BoxComponent and set as RootComponent for the Actor
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	BoxCollision->SetupAttachment(SkeletalMesh);

	// Créer et attacher le composant audio
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->VolumeMultiplier = 100.f;

	////////////////////////////////////////////
	
	if (SkeletalMesh)
	{
		// Load the lever's mesh
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> LeverAsset(TEXT("/Game/Assets/Decors/Level_Mechanics/Lever1.Lever1"));
		if (LeverAsset.Succeeded())
		{
			SkeletalMesh->SetSkeletalMesh(LeverAsset.Object);
			UE_LOG(LogTemp, Warning, TEXT("Loaded SkeletalMesh for Lever"));
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Failed to load keletalMesh for Lever"));
		}
		// Rotate the SkeletalMesh
		SkeletalMesh->SetRelativeRotation(FRotator(0, 90, 0));
	}
	if (BoxCollision)
	{
		// Move and resize the BoxCollision
		BoxCollision->SetRelativeLocation(FVector(0, 110, 90));
		BoxCollision->SetWorldScale3D(FVector(3.5, 3.5, 3.5));
	}
	
	// Load animations
	static ConstructorHelpers::FObjectFinder<UAnimSequence> LeverOffAnimAsset(TEXT("/Game/Assets/Decors/Level_Mechanics/Lever_Anim_Lever_Off.Lever_Anim_Lever_Off"));
	if (LeverOffAnimAsset.Succeeded())
	{
		LeverOffAnim = LeverOffAnimAsset.Object;
		UE_LOG(LogTemp, Warning, TEXT("Loaded Animation for Lever Off"));
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Failed to load Animation for Lever Off"));
	}
	static ConstructorHelpers::FObjectFinder<UAnimSequence> LeverOnAnimeAsset(TEXT("/Game/Assets/Decors/Level_Mechanics/Lever_Anim_Lever_On.Lever_Anim_Lever_On"));
	if (LeverOnAnimeAsset.Succeeded())
	{
		LeverOnAnim = LeverOnAnimeAsset.Object;
		UE_LOG(LogTemp, Warning, TEXT("Loaded Animation for Lever On"));
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Failed to load Animation for Lever On"));
	}

	// Load the sound
	static ConstructorHelpers::FObjectFinder<USoundBase> LeverActivationSoundAsset(TEXT("/Game/Assets/Decors/Level_Mechanics/SFX/LeverActivation.LeverActivation"));
	if (LeverActivationSoundAsset.Succeeded())
	{
		LeverActivationSound = LeverActivationSoundAsset.Object;
		UE_LOG(LogTemp, Warning, TEXT("Loaded Sound for Lever Activation"));
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Failed to load Sound for Lever Activation"));
	}
	if (AudioComponent)
	{
		// Apply the sound to the AudioComponent
		AudioComponent->SetSound(LeverActivationSound);
	}

	GroupId = 0;
}

// Called when the game starts or when spawned
void ALever::BeginPlay()
{
	Super::BeginPlay();
	if (!LeverOffAnim)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Animation Lever_Anim_Lever_Off non assignée dans l'éditeur !"));
	}
	if (!LeverOnAnim)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Animation Lever_Anim_Lever_On non assignée dans l'éditeur !"));
	}
	UAnimSequence* AnimationToPlay = bIsActivated ? LeverOffAnim : LeverOnAnim;
	TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
	GetComponents<USkeletalMeshComponent>(SkeletalMeshComponents);

	for (USkeletalMeshComponent* SkeletalMeshComponent : SkeletalMeshComponents)
	{
		if (SkeletalMeshComponent && AnimationToPlay)
		{
			// Jouer l'animation pour chaque SkeletalMeshComponent
			SkeletalMeshComponent->PlayAnimation(AnimationToPlay, false);
		}
	}
}

// Called every frame
void ALever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALever::ChangeState(bool bIsEmiter)
{
	// Play the switch animation
	UAnimSequence* AnimationToPlay = bIsActivated ? LeverOnAnim : LeverOffAnim;
	TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
	GetComponents<USkeletalMeshComponent>(SkeletalMeshComponents);

	for (USkeletalMeshComponent* SkeletalMeshComponent : SkeletalMeshComponents)
	{
		if (SkeletalMeshComponent && AnimationToPlay)
		{
			// Jouer l'animation pour chaque SkeletalMeshComponent
			SkeletalMeshComponent->PlayAnimation(AnimationToPlay, false);
		}
	}

	// Change the state of the Lever
	bIsActivated = !bIsActivated;
	
	// Update the state of the Mechanism linked to the Lever
	for (TActorIterator<AMechanism> It(GetWorld()); It; ++It) 
	{
		AMechanism* Mechanism = *It;
		if (Mechanism && Mechanism->LinkedLever == this)
		{
			Mechanism->UpdateState();
			// log the state of the mechanism
			FString StateM = Mechanism->bIsActivated ? "Activated" : "Deactivated";
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Mechanism is %s"), *StateM));
			
		}
	}
	
	if (bIsEmiter) {
		AdaptGroupState();
		// Play the sound
		if (LeverActivationSound && AudioComponent)
		{
			AudioComponent->Play(); 
		}
	}
}

bool ALever::GetLeverState()
{
	return bIsActivated;
}

void ALever::AdaptGroupState()
{
	if (GroupId != -1) {
		// Get all lever of the scene
		TArray<AActor*> FoundLevers;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALever::StaticClass(), FoundLevers);
			
		if (this->GroupType == ELeverGroupType::OnlyOneDeactivated)
		{
			for (AActor* Actor : FoundLevers)
			{
				ALever* Lever = Cast<ALever>(Actor);
				if (Lever && Lever != this) 
				{
					if (Lever->GroupId == this->GroupId && !Lever->bIsActivated) 
					{
						Lever->ChangeState(false);
					}
				}
			}
		}

		if (this->GroupType == ELeverGroupType::OnlyOneActivated)
		{
			for (AActor* Actor : FoundLevers)
			{
				ALever* Lever = Cast<ALever>(Actor);
				if (Lever && Lever != this) 
				{
					if (Lever->GroupId == this->GroupId && Lever->bIsActivated) 
					{
						Lever->ChangeState(false);
					}
				}
			}
		}
	}
}