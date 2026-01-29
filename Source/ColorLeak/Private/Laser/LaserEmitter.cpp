#include "Laser/LaserEmitter.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Chromarion.h"
#include "DrawDebugHelpers.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Laser/LaserReceiver.h"
#include "Laser/ReflectiveBlock.h"

ALaserEmitter::ALaserEmitter()
{
	// Create a simple mesh for the laser source (optional)
	LaserMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaserMesh"));
	RootComponent = LaserMesh;

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(RootComponent);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);

	EmissiveSphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EmissiveSphereMesh"));
	EmissiveSphereMesh->SetupAttachment(RootComponent);

	this->Tags.Add("Paintable");
}

void ALaserEmitter::HideLaser(int FromBounceCount)
{
	for (int i = FromBounceCount; i < MaxReflections + 1; i++)
	{
		if (LaserComponents[i] != nullptr)
		{
			LaserComponents[i]->SetVisibility(false);
		}
	}
}

void ALaserEmitter::BeginPlay()
{
	Super::BeginPlay();

	DynamicMaterial = EmissiveSphereMesh->CreateAndSetMaterialInstanceDynamic(0);

	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue("BaseColor", FromColor(ColorType));
		DynamicMaterial->SetScalarParameterValue("EmissiveFactor", 10.0f);
	}

	if (LaserSystem)
	{
		for (int i = 0; i < MaxReflections + 1; i++)
		{
			UNiagaraComponent* LaserEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), LaserSystem, FVector::ZeroVector, FRotator::ZeroRotator);
			LaserEffect->SetVisibility(false);
			LaserComponents.Add(LaserEffect);
		}

		if (!bIsStatic)
		{
			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(TimerHandle, this, &ALaserEmitter::TimerCallback, RefreshRate, true);
		} else
		{
			TimerCallback();
		}
	}
}

void ALaserEmitter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	for (int i = 0; i < MaxReflections + 1; i++)
	{
		if (LaserComponents[i] != nullptr)
		{
			LaserComponents[i]->Deactivate();
			LaserComponents[i]->DestroyComponent();
		}
	}

	if (!bIsStatic)
	{
		GetWorldTimerManager().ClearAllTimersForObject(this);
	}
}

void ALaserEmitter::TimerCallback()
{
	ALaserReceiver* Receiver = FireLaser(ArrowComponent->GetComponentLocation(), ArrowComponent->GetForwardVector(), 0, this, ColorType);

	if (Receiver == nullptr && LaserReceiver)
	{
		LaserReceiver->DeactivateMechanism();
		LaserReceiver = nullptr;
	}
	else if (Receiver != LaserReceiver)
	{
		if (LaserReceiver) LaserReceiver->DeactivateMechanism();
		LaserReceiver = Receiver;
	}

	// new player hit
	if (bHitPlayer && !bHitPlayerLastTick && HitPlayer)
	{
		// vibration
		bHitPlayerLastTick = true;
		HitPlayerLastTick = HitPlayer;
		if (APlayerController* PC = Cast<APlayerController>(HitPlayer->GetController()))
		{
			if (ForceFeedbackEffectHitLaser)
			{
				PC->ClientPlayForceFeedback(ForceFeedbackEffectHitLaser);
			}
		}
	}
	if (bHitPlayerLastTick && !bHitPlayer)
	{
		bHitPlayerLastTick = false;
		HitPlayerLastTick = nullptr;
	}
	if (bHitPlayer && HitPlayer != HitPlayerLastTick)
	{
		bHitPlayerLastTick = true;
		HitPlayerLastTick = HitPlayer;

		if (APlayerController* PC = Cast<APlayerController>(HitPlayer->GetController()))
		{
			if (ForceFeedbackEffectHitLaser)
			{
				PC->ClientPlayForceFeedback(ForceFeedbackEffectHitLaser);
			}
		}
	}

	bHitPlayer = false;
	HitPlayer = nullptr;
}

ALaserReceiver* ALaserEmitter::FireLaser(FVector Start, FVector Direction, int32 BounceCount, AActor* OwnerActor,
                                         EColorType Color)
{
	if (BounceCount >= MaxReflections) return nullptr;

	FVector End = Start + (Direction * LaserLength);
	FHitResult HitResult;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerActor);

	// Perform a line trace (raycast)
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel1, Params);

	if (bHit)
	{
		if (LaserComponents[BounceCount] != nullptr)
		{
			LaserComponents[BounceCount]->SetWorldLocation(Start);
			LaserComponents[BounceCount]->SetVectorParameter(FName("LaserEnd"), HitResult.ImpactPoint);
			LaserComponents[BounceCount]->SetColorParameter(FName("LaserColor"), FromColor(Color)); // Set the color for no hit scenario
			LaserComponents[BounceCount]->SetVisibility(true);
		}

		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			if (HitActor->ActorHasTag("ReflectiveBlock"))
			{
				if (auto ReflectiveBlock = Cast<AReflectiveBlock>(HitActor))
				{
					if (Color == EColorType::Uncolored || Color == ReflectiveBlock->GetColorType())
					{
						FVector ReflectDir = FMath::GetReflectionVector(Direction, HitResult.ImpactNormal);
						return FireLaser(HitResult.ImpactPoint, ReflectDir, BounceCount + 1, ReflectiveBlock, Color);
					} else
					{
						HideLaser(BounceCount + 1);
					}
				}
			}
			else if (HitActor->ActorHasTag("LaserReceiver"))
			{
				if (auto Receiver = Cast<ALaserReceiver>(HitActor))
				{
					if (Receiver != LaserReceiver)
					{
						Receiver->ActivateMechanism(Color);
					}

					HideLaser(BounceCount + 1);

					return Receiver;
				}
			}
			else if (HitActor->ActorHasTag("Chromarion"))
			{
				if (auto Chromarion = Cast<AChromarion>(HitActor))
				{

					bHitPlayer = true;
					HitPlayer = Chromarion;

					if (Chromarion->GetCurrentColor() == EColorType::Purple)
					{
						FVector ReflectDir = FMath::GetReflectionVector(Direction, HitResult.ImpactNormal);
						// remove the z component
						ReflectDir.X = 0;
						return FireLaser(HitResult.ImpactPoint, ReflectDir, BounceCount + 1,Chromarion, Color);
					} else
					{
						HideLaser(BounceCount + 1);
					}
				}
			}else
			{
				HideLaser(BounceCount + 1);
			}


			if (HitActor->ActorHasTag("Player"))
			{
				bHitPlayer = true;
				HitPlayer = Cast<AColorLeakCharacter>(HitActor);
			}
		}
	}
	else
	{
		if (LaserComponents[BounceCount] != nullptr)
		{
			LaserComponents[BounceCount]->SetWorldLocation(Start);
			LaserComponents[BounceCount]->SetVectorParameter(FName("LaserEnd"), End);
			LaserComponents[BounceCount]->SetColorParameter(FName("LaserColor"), FromColor(Color)); // Set the color for no hit scenario
			LaserComponents[BounceCount]->SetVisibility(true);
		}

		HideLaser(BounceCount + 1);
	}

	return nullptr;
}

void ALaserEmitter::SetCurrentColor(EColorType newColor)
{
	IPaintableInterface::SetCurrentColor(newColor);

	ColorType = newColor;

	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue("BaseColor", FromColor(newColor));
	}
}
