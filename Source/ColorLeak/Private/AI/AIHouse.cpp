// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIHouse.h"

#include "AIController.h"
#include "EngineUtils.h"
#include "AI/AIPaintEater.h"
#include "Mechanism/Mechanism.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Objects/RightPlateform.h"

// Sets default values
AAIHouse::AAIHouse()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	BoxCollision->SetupAttachment(StaticMesh);

}

// Called when the game starts or when spawned
void AAIHouse::BeginPlay()
{
	Super::BeginPlay();
	this->OnActorBeginOverlap.AddDynamic(this, &AAIHouse::OnOverlap);
	
}

void AAIHouse::OnOverlap(AActor* MyActor, AActor* OtherActor)
{
	if (auto AI = Cast<AAIPaintEater>(OtherActor))
	{
		if (AI->bIsChasing)
		{
			GetWorld()->GetTimerManager().ClearTimer(AI->WigdetPositionTimerHandle);
			AI->ProgressBarWidget->Destruct();
			EndAnimation(AI);
		}
	}
}

// Called every frame
void AAIHouse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAIHouse::ChangeState()
{
	// Change the state of the Lever
	bIsActivated = !bIsActivated;
	
	// Update the state of the Mechanism linked to the Lever
	for (TActorIterator<AMechanism> It(GetWorld()); It; ++It) 
	{
		AMechanism* Mechanism = *It;
		if (Mechanism && Mechanism->LinkedHouse == this)
		{
			if (auto Platform = Cast<ARightPlateform>(Mechanism))
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Platform"));
				Platform->UpdateState();
			}
			else
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Else"));
				Mechanism->UpdateState();
			}
			// log the state of the mechanism
			//FString StateM = Mechanism->bIsActivated ? "Activated" : "Deactivated";
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("1.Mechanism is %s"), *StateM));
		}
	}
}

void AAIHouse::EndAnimation(AAIPaintEater* AI)
{
	if (!AI) return;

	AI->GetCharacterMovement()->bConstrainToPlane = false;
	
	// Récupération du contrôleur d'IA
	AAIController* AIController = Cast<AAIController>(AI->GetController());
	if (!AIController) return;

	AI->SetIsChasing(false);
	float ZOffSet = 30.f;
	AIController->StopMovement();

	// Désactiver la gravité
	AI->SetGravity(false);
	
	FVector InsideLocation = FVector(GetActorLocation().X+200, GetActorLocation().Y+30, AI->GetActorLocation().Z); // À l'intérieur (disparaît)
	FVector RoofLocation = GetActorLocation() + FVector(0, 0, 260 + ZOffSet); // Sur le toit

	AI->SetActorRotation(FRotator(0, 0, 0));
	AIController->MoveToLocation(InsideLocation);
	
	GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle, [this, AI, ZOffSet]() 
	{
		// Après 1 seconde, téléporter l'IA sur le toit
		FVector RoofLocation = GetActorLocation() + FVector(0, 0, 260 + ZOffSet);
		AI->SetActorLocation(RoofLocation);  // Téléportation immédiate à la position du toit
        
		// Optionnel : on peut aussi réinitialiser la rotation de l'IA si nécessaire
		AI->SetActorRotation(FRotator(0, 90, 0));
        ChangeState();
		// Stopper le timer une fois l'action effectuée
		GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
		UpdateFlagColor(AI->Color);
	}, 2.0f, false);
}

void AAIHouse::UpdateFlagColor(EColorType Color)
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

	FString MainMaterialPath = FString::Printf(TEXT("Material'/Game/Characters/Chromarion/Materials/%s/M_Main.M_Main'"), *ColorName);
	UMaterialInterface* Main = LoadObject<UMaterialInterface>(nullptr, *MainMaterialPath);

	if (Main)
	{
		StaticMesh->SetMaterial(6, Main);
	}
}
