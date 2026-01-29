// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/CheckPoint.h"

#include "Acryl.h"
#include "Chromarion.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"


// Sets default values
ACheckPoint::ACheckPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	BoxCollision->SetupAttachment(StaticMesh);

	CheckPointType = ECheckPointType::Both;

	// Créer le composant Niagara
	NiagaraSystemCheckPoint = CreateDefaultSubobject<UNiagaraComponent>(TEXT("CheckpointNiagara"));
    
	// Attacher au root component
	NiagaraSystemCheckPoint->SetupAttachment(RootComponent);
    
	// Désactiver au début
	NiagaraSystemCheckPoint->SetAutoActivate(false);

}

// Called when the game starts or when spawned
void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();

	// Bind function OnActorBeginOverlap with your class function OnOverlap
	this->OnActorBeginOverlap.AddDynamic(this, &ACheckPoint::OnOverlap);

	changeColor(EColorType::Uncolored);
	bIsActive = false;
}

void ACheckPoint::OnOverlap(AActor* MyActor, AActor* OtherActor)
{
	FVector NewRespawnLocation = GetActorLocation();
	if (!bIsActive) {
		if (CheckPointType == ECheckPointType::Both)
		{
			if (AAcryl* Acryl = Cast<AAcryl>(OtherActor))
			{
				Acryl->RespawnLocation = FVector(Acryl->GetActorLocation().X,GetActorLocation().Y,GetActorLocation().Z+100);
				AChromarion* Chromarion2 = Cast<AChromarion>(UGameplayStatics::GetActorOfClass(GetWorld(), AChromarion::StaticClass()));
				if (Chromarion2)
				{
					Chromarion2->RespawnLocation = FVector(Chromarion2->GetActorLocation().X,GetActorLocation().Y,GetActorLocation().Z+80);
				}

				changeColor(Acryl->GetColorBrush("Socket_Brush"));
			}
			else if (AChromarion* Chromarion = Cast<AChromarion>(OtherActor))
			{
				Chromarion->RespawnLocation = FVector(Chromarion->GetActorLocation().X,GetActorLocation().Y,GetActorLocation().Z+80);
				AAcryl* Acryl2 = Cast<AAcryl>(UGameplayStatics::GetActorOfClass(GetWorld(), AAcryl::StaticClass()));
				if (Acryl2)
				{
					Acryl2->RespawnLocation = FVector(Acryl2->GetActorLocation().X,GetActorLocation().Y,GetActorLocation().Z+100);
				}
				
				changeColor(Chromarion->GetCurrentColor());
			} 
		}
		else if (CheckPointType == ECheckPointType::Single)
		{
			// Update Acryl's checkpoint 
			if (auto Acryl = Cast<AAcryl>(OtherActor))
			{
				Acryl->RespawnLocation = FVector(Acryl->GetActorLocation().X,GetActorLocation().Y,GetActorLocation().Z+100);
				changeColor(Acryl->GetColorBrush("Socket_Brush"));
			}
			// Update Chromarion's checkpoint 
			else if (auto Chromarion = Cast<AChromarion>(OtherActor))
			{
				Chromarion->RespawnLocation = FVector(Chromarion->GetActorLocation().X,GetActorLocation().Y,GetActorLocation().Z+80);
				changeColor(Chromarion->GetCurrentColor());
			}
		}
	}

	if (CheckPointSound && !bIsActive)
	{
		UGameplayStatics::PlaySoundAtLocation(this, CheckPointSound, GetActorLocation());
	}

	if (NiagaraSystemCheckPoint && !bIsActive)
	{
		NiagaraSystemCheckPoint->SetActive(true);
	}

	if (CheckPointColor == EColorType::Uncolored)
	{
		if (auto Acryl = Cast<AAcryl>(OtherActor))
		{
			changeColor(Acryl->GetColorBrush("Socket_Brush"));
		}
		else if (auto Chromarion = Cast<AChromarion>(OtherActor))
		{
			changeColor(Chromarion->GetCurrentColor());
		}
	}

	bIsActive = true;
}

void ACheckPoint::UpdateRespawnLocation(AColorLeakCharacter* Character, const FVector& NewLocation)
{
	if (Character)
	{
		Character->RespawnLocation = FVector(Character->GetActorLocation().X, NewLocation.Y, NewLocation.Z);
	}
}

void ACheckPoint::changeColor(EColorType newColor)
{
	FString ColorName;
	CheckPointColor = newColor;
	
	switch (newColor)
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

	if (Main )
	{
		StaticMesh->SetMaterial(1, Main);
	}
}


