#include "Objects/Water.h"
#include "TimerManager.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWater::AWater()
{
	PrimaryActorTick.bCanEverTick = false; 
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(StaticMesh);
	
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);

	FallingSpeed = 200.0f; 
	MoveInterval = 0.02f;

	bIsDynamic = false;
}

void AWater::BeginPlay()
{
	Super::BeginPlay();
	this->OnActorBeginOverlap.AddDynamic(this, &AWater::OnOverlap);
    
	StaticMesh->SetSimulatePhysics(false); 

	// Start falling movement
	if (bIsDynamic)
	{
		GetWorldTimerManager().SetTimer(MoveTimer, this, &AWater::MoveDown, MoveInterval, true);
	}
}

void AWater::MoveDown()
{
	FVector NewLocation = GetActorLocation();
	// Linear movement 
	NewLocation.Z -= FallingSpeed * MoveInterval; 
	SetActorLocation(NewLocation);
}

void AWater::SetFallingSpeed(float Speed)
{
	FallingSpeed = Speed;
	
	MoveInterval = 0.02f; 
	GetWorldTimerManager().SetTimer(MoveTimer, this, &AWater::MoveDown, MoveInterval, true);
}

void AWater::OnOverlap(AActor* MyActor, AActor* OtherActor)
{
	bool bPlaySound= false;

	if (auto Chromarion = Cast<AChromarion>(OtherActor))
	{
		if (Chromarion->GetCurrentColor() != EColorType::Uncolored) { bPlaySound = true; }
		Chromarion->ResetGravity();
		Chromarion->SetCurrentColor(EColorType::Uncolored);
	}
	if (auto Acryl = Cast<AAcryl>(OtherActor))
	{
		if (Acryl->GetColorBrush("Socket_Brush") != EColorType::Uncolored) { bPlaySound = true; }
		Acryl->ClearBrush();
	}
	if (auto ColoredBox = Cast<AColoredBox>(OtherActor))
	{
		if (ColoredBox->BoxColor != EColorType::Uncolored) { bPlaySound	= true; }
		ColoredBox->SetCurrentColor(EColorType::Uncolored);
	}

	// Play splash sound
	if (bPlaySound&& SplashSound) {	UGameplayStatics::PlaySoundAtLocation(this, SplashSound, GetActorLocation());}
}

