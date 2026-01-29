// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Circle.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Objects/BlockPath.h"

// Sets default values
ACircle::ACircle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	BoxCollision->SetupAttachment(StaticMesh);

	CircleColor = EColorType::Uncolored;
}

// Called when the game starts or when spawned
void ACircle::BeginPlay()
{
	Super::BeginPlay();

	// Bind function OnActorBeginOverlap with your class function OnOverlap
	this->OnActorBeginOverlap.AddDynamic(this, &ACircle::OnOverlap);

	// Bind function OnActorBeginOverlap with your class function EndOverlap
	this->OnActorEndOverlap.AddDynamic(this, &ACircle::OnEndOverlap);

	if (UMeshComponent* Mesh = FindComponentByClass<UMeshComponent>())
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if(bIsActive){
		UpdateMaterial();
	}
}

void ACircle::OnOverlap(AActor* MyActor, AActor* OtherActor){

	if(OtherActor && bIsActive && !bIsValidate){
	
		bIsValidate = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Circle validate"));
		if (UMeshComponent* Mesh = FindComponentByClass<UMeshComponent>())
		{
			Mesh->SetVisibility(false);
			Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		}

		if(ValidateSound){
			UGameplayStatics::SpawnSoundAttached(ValidateSound, StaticMesh);
		}

		if(BlockPath){
			BlockPath->OpenPath();
		}
		if (Mechanism)
		{
			Mechanism->UpdateState();
		}

	}
}

void ACircle::OnEndOverlap(AActor* MyActor, AActor* OtherActor){

}

// Called every frame
void ACircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACircle::SetCurrentColor(EColorType newColor){
	CircleColor = newColor;
	UpdateMaterial();
	bIsActive = true;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("SetCurrentColor !"));

}

void ACircle::UpdateMaterial()
{
	FString ColorName;

	switch (CircleColor)
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
		// Apply material to ColoredBox
		StaticMesh->SetMaterial(0, Main);
	}
}

