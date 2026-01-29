#include "Objects/Teleporter.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "ColorLeak/ColorLeakCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Dialog/DialogManager.h"

// Sets default values
ATeleporter::ATeleporter()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(Mesh);

	// Create Niagara Component but don't activate it yet
	TeleportEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TeleportEffect"));
	TeleportEffectComponent->SetupAttachment(RootComponent);
	TeleportEffectComponent->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void ATeleporter::BeginPlay()
{
	Super::BeginPlay();

	this->OnActorBeginOverlap.AddDynamic(this, &ATeleporter::OnOverlap);
	this->OnActorEndOverlap.AddDynamic(this, &ATeleporter::OnEndOverlap);

	// check if there is already in the teleporter
	// get overlapping actors
	TArray<AActor*> OverlappingActors;
	BoxCollision->GetOverlappingActors(OverlappingActors);
	if (OverlappingActors.Num() > 0)
	{
		for (AActor* Actor : OverlappingActors)
		{
			if (Actor)
			{
				bHasActor = true;
				Actor_ = Actor;
				break;
			}
		}
	}
}

void ATeleporter::OnOverlap(AActor* MyActor, AActor* OtherActor)
{
	if (!MyActor || !OtherActor) return;
	if (bHasActor) return;

	bHasActor = true;
	Actor_ = OtherActor;

	UGameplayStatics::PlaySoundAtLocation(this, OverlapTeleporterSound, GetActorLocation());

	// If linked teleporter also has a character, enable the effect
	if (HasLinkedTeleporter() && LinkedTeleporter->HasActor())
	{
		StartEffect();
		LinkedTeleporter->StartEffect();
	}

	// Start the teleportation process
	if (!bHasAlreadyTeleport)
	{
		bTeleportationStarted = true;
		GetWorldTimerManager().SetTimer(TeleporterTimer, [this]()
		{
			bTeleportationStarted = false;
			TeleportCharacter(Actor_);
		}, TeleportationDelay, false);
	}
}

void ATeleporter::OnEndOverlap(AActor* MyActor, AActor* OtherActor)
{
	if (!MyActor || !OtherActor) return;
	if (OtherActor != Actor_) return;

	bHasActor = false;
	Actor_ = nullptr;

	// Stop the effect when a player leaves
	StopEffect(true);
	if (HasLinkedTeleporter())
	{
		LinkedTeleporter->StopEffect(true);
	}

	// Cancel teleport if it was about to happen
	if (bTeleportationStarted)
	{
		GetWorldTimerManager().ClearTimer(TeleporterTimer);
	}

	bHasAlreadyTeleport = false;
}

void ATeleporter::StartEffect()
{
	if (TeleportEffectComponent)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ChargingTeleportationSound, GetActorLocation());
		TeleportEffectComponent->Activate();
		TeleportEffectComponent->SetVisibility(true);
	}
}

void ATeleporter::StopEffect(bool bForce)
{
	if (TeleportEffectComponent)
	{
		TeleportEffectComponent->Deactivate();
		if (bForce)
		{
			TeleportEffectComponent->SetVisibility(false);
		}
	}
}

void ATeleporter::TeleportCharacter(AActor* Player)
{
	if (HasLinkedTeleporter() && LinkedTeleporter->HasActor())
	{
		AActor* OtherCharacter = LinkedTeleporter->GetActor();
		if (OtherCharacter)
		{
			UGameplayStatics::PlaySoundAtLocation(this, TeleportingSound, GetActorLocation());
			
			Player->SetActorEnableCollision(false);
			OtherCharacter->SetActorEnableCollision(false);

			// Swap positions
			FTransform MyCharacterTransform = Player->GetActorTransform();
			Player->SetActorTransform(OtherCharacter->GetActorTransform(), false, nullptr, ETeleportType::TeleportPhysics);
			OtherCharacter->SetActorTransform(MyCharacterTransform, false, nullptr, ETeleportType::TeleportPhysics);

			// Update teleport state
			Actor_ = OtherCharacter;
			bHasAlreadyTeleport = true;
			bHasActor = true;

			LinkedTeleporter->SetActor(Player);
			LinkedTeleporter->bHasAlreadyTeleport = true;
			LinkedTeleporter->bHasActor = true;

			Player->SetActorEnableCollision(true);
			OtherCharacter->SetActorEnableCollision(true);

			StopEffect();
			LinkedTeleporter->StopEffect();
		}
	}
}

// Called every frame
void ATeleporter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
