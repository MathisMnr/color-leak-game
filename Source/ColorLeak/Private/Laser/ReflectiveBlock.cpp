#include "Laser/ReflectiveBlock.h"

AReflectiveBlock::AReflectiveBlock()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create the mesh for the reflective block
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh"));
	RootComponent = BlockMesh;

	// Add a tag so the laser knows this is reflective
	this->Tags.Add("ReflectiveBlock");
	this->Tags.Add("Paintable");
}

void AReflectiveBlock::BeginPlay()
{
	Super::BeginPlay();

	DynamicMaterial = BlockMesh->CreateAndSetMaterialInstanceDynamic(0);

	// change the base color
	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue("BaseColor", FromColor(ColorType));
	}
}

void AReflectiveBlock::SetCurrentColor(EColorType newColor)
{
	IPaintableInterface::SetCurrentColor(newColor);

	ColorType = newColor;

	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue("BaseColor", FromColor(newColor));
	}
}
