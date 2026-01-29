#include "Objects/ColoredBox.h"
#include "Chromarion.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

AColoredBox::AColoredBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	BoxCollision->SetupAttachment(StaticMesh);
	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));
	ArrowMesh->SetupAttachment(StaticMesh);
	
	BoxColor = EColorType::Uncolored;
	
	// Load the mesh "/Game/Assets/Decors/Cubes/Cube_Crate"
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Game/Assets/Decors/Cubes/Cube_Crate"));
	if (CubeAsset.Succeeded())
	{
		StaticMesh->SetStaticMesh(CubeAsset.Object);
		//UE_LOG(LogTemp, Warning, TEXT("Loaded StaticMesh for ColoredBox"));
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Failed to load StaticMesh for ColoredBox"));
	}
	
	StaticMesh->SetWorldScale3D(FVector(0.8f, 0.8f, 0.8f));				// Apply 0.8 scale
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	BoxOffset = FVector(0,200,0);
}

void AColoredBox::BeginPlay()
{
	Super::BeginPlay();
	SetCurrentColor(BoxColor);
	UpdateMaterial();

	// Bind function OnActorBeginOverlap with your class function OnOverlap
	this->OnActorBeginOverlap.AddDynamic(this, &AColoredBox::OnOverlap);

	// Bind function OnActorBeginOverlap with your class function EndOverlap
	this->OnActorEndOverlap.AddDynamic(this, &AColoredBox::OnEndOverlap);

	if (bIsMain)
	{
		UpdateLinkedBoxPosition();
		UpdateLinkedBoxScale();
	}
	
	SetBoxMass(1000000.0f);
	InvertGravity(BoxColor==EColorType::Green, true);
}

void AColoredBox::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(BoxSyncTimerHandle);
}

// =============================================================================================
//                                       Overlap Functions
// =============================================================================================
void AColoredBox::OnOverlap(AActor* MyActor, AActor* OtherActor)
{
	if (auto Chromarion = Cast<AChromarion>(OtherActor))
	{
		if (IsSameColor(Chromarion->GetCurrentColor()))
		{
			SetBoxMass(100.0f);
			if (LinkedBox)
			{
				GetWorld()->GetTimerManager().SetTimer(BoxSyncTimerHandle, [this]()
				{
					GetWorld()->GetTimerManager().ClearTimer(DelayedStopTimerHandle);
					UpdateLinkedBoxPosition();
				}, 0.01f, true);
			}
		}
	}
}

void AColoredBox::OnEndOverlap(AActor* MyActor, AActor* OtherActor)
{
	SetBoxMass(1000000.0f); // Remet la masse à 100 lorsque plus personne ne touche
	if (LinkedBox)
	{
		GetWorld()->GetTimerManager().SetTimer(DelayedStopTimerHandle, [this]()
		{
			GetWorld()->GetTimerManager().ClearTimer(BoxSyncTimerHandle);
		}, 2.0f, false);
	}
}

void AColoredBox::SetBoxMass(float NewMass)
{
	if (UPrimitiveComponent* BoxComponent = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Mass: %f "),NewMass) );
		BoxComponent->SetMassOverrideInKg(NAME_None, NewMass, true);
	}
}

// =============================================================================================
//                                    Linked Box Functions
// =============================================================================================

void AColoredBox::UpdateLinkedBoxPosition()
{
	// check if actor is ready
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("UpdateLinkedBoxPosition")));
	if (LinkedBox)
	{
		FVector CurrentLocation = GetActorLocation();
		LinkedBox->SetActorLocation(CurrentLocation + BoxOffset);
	}
}

void AColoredBox::UpdateLinkedBoxScale()
{
	if (LinkedBox)
	{
		LinkedBox->BoxColor = this->BoxColor;
		FVector MeshScale = StaticMesh->GetComponentScale();
		LinkedBox->SetActorScale3D(MeshScale);
	}
}

// =============================================================================================
//                                       Other Functions
// =============================================================================================

bool AColoredBox::IsSameColor(EColorType OtherColor)
{
	// if color is the same AND color is not uncolored
	if (BoxColor == OtherColor && (BoxColor != EColorType::Uncolored || OtherColor != EColorType::Uncolored))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void AColoredBox::SetCurrentColor(EColorType newColor){
	BoxColor = newColor;
	UpdateMaterial();
	InvertGravity(BoxColor==EColorType::Green, false);
}

void AColoredBox::UpdateMaterial()
{
	FString ColorName;

	switch (BoxColor)
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

	FString SecondaryMaterialPath = FString::Printf(TEXT("Material'/Game/Characters/Chromarion/Materials/%s/M_Secondary.M_Secondary'"), *ColorName);
	UMaterialInterface* Secondary = LoadObject<UMaterialInterface>(nullptr, *SecondaryMaterialPath);

	if (Main && Secondary)
	{
		// Apply material to ColoredBox
		StaticMesh->SetMaterial(0, Main);
		StaticMesh->SetMaterial(1, Secondary);
	}
	if (ArrowMesh)
	{
		ArrowMesh->SetMaterial(0, Main);
	}
}

void AColoredBox::InvertGravity(bool bIsGravityEnable, bool bIsOnLaunch)
{
	StaticMesh->SetEnableGravity(!bIsGravityEnable);
	StaticMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
	if (bIsGravityEnable && !bIsOnLaunch)
	{
		StaticMesh->AddForce(FVector(0.0f, 0.0f, StaticMesh->GetMass() * 980.0f * 100)); 
	}
}
