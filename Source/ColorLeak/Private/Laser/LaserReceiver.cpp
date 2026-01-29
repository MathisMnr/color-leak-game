#include "Laser/LaserReceiver.h"

ALaserReceiver::ALaserReceiver()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create the mesh for the receiver
	ReceiverMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ReceiverMesh"));
	RootComponent = ReceiverMesh;

	ReceiverBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ReceiverBox"));
	ReceiverBox->SetupAttachment(ReceiverMesh);

	// Add a tag so the laser can detect it
	this->Tags.Add("LaserReceiver");
}

void ALaserReceiver::BeginPlay()
{
	Super::BeginPlay();
}

void ALaserReceiver::ActivateMechanism(EColorType Color)
{
}

void ALaserReceiver::DeactivateMechanism()
{
}
