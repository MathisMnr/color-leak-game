// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/GravityComponent.h"


UGravityComponent::UGravityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGravityComponent::BeginPlay()
{
	Super::BeginPlay();

	UPrimitiveComponent* PrimitiveComp = GetOwner()->FindComponentByClass<UPrimitiveComponent>();
	if (PrimitiveComp)
	{
		PrimitiveComp->SetEnableGravity(false); // Disable built-in gravity
		PrimitiveComp->SetSimulatePhysics(true); // Ensure physics is on
	}
}

void UGravityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Owner = GetOwner();
	if (!Owner) return;

	UPrimitiveComponent* PrimitiveComponent = Owner->FindComponentByClass<UPrimitiveComponent>();
	if (!PrimitiveComponent || !PrimitiveComponent->IsSimulatingPhysics()) return;

	// Apply continuous force like real gravity
	FVector GravityForce = GravityDirection * GravityStrength * PrimitiveComponent->GetMass();
	PrimitiveComponent->AddForce(GravityForce, NAME_None, true);
}

void UGravityComponent::InvertGravity()
{
	GravityDirection *= -1.0f; // Flip gravity direction
}