#include "Chromarion.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "ColorLeak/ColorLeakCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Widget/PowerUpWidget.h"

AChromarion::AChromarion()
{
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->AirControl = 1.0f; // Permet de changer de direction librement en l'air
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->FallingLateralFriction = 0.1f;  // Réduisez cette valeur pour plus de réactivité
	GetCharacterMovement()->JumpZVelocity = 800.f;  // Augmenter cette valeur pour un saut plus haut
	GetCharacterMovement()->GravityScale = 1.0f; // Augmenter cette valeur pour une chute plus rapide

	// Default color 
	CurrentColor = EColorType::Uncolored;

	// We load the sound effects
	JumpSoundOne = LoadObject<USoundBase>(nullptr, TEXT("/Game/Characters/Chromarion/SFX/Jump_1"));
	JumpSoundTwo = LoadObject<USoundBase>(nullptr, TEXT("/Game/Characters/Chromarion/SFX/Jump_2"));
	LongJumpSound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Characters/Chromarion/SFX/Jump_3"));
	LandSound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Characters/Chromarion/SFX/Footstep2"));

	SprintTimeWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("SprintTimeWidgetComponent"));
	SprintTimeWidgetComponent->SetupAttachment(RootComponent);

	ShrinkPositionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("ShrinkPositionWidgetComponent"));
	ShrinkPositionWidgetComponent->SetupAttachment(RootComponent);

	DashTrailComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DashTrailComponent"));
	DashTrailComponent->SetupAttachment(GetMesh());
	DashTrailComponent->bAutoActivate = false;

	SpeedTrailComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SpeedTrailComponent"));
	SpeedTrailComponent->SetupAttachment(GetMesh());
	SpeedTrailComponent->bAutoActivate = false;

	JumpEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("JumpEffectComponent"));
	JumpEffectComponent->SetupAttachment(GetMesh());
	JumpEffectComponent->bAutoActivate = false;

	GravityEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("GravityEffectComponent"));
	GravityEffectComponent->SetupAttachment(GetMesh());
	GravityEffectComponent->bAutoActivate = false;
}

void AChromarion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ScaleEffectTimeline.TickTimeline(DeltaTime);
}


void AChromarion::HandleScaling(float Value)
{
	FVector NewScale = FMath::Lerp(FVector(1.f), FVector(0.48f), Value);
	GetCapsuleComponent()->SetWorldScale3D(NewScale);
}

void AChromarion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		// special input that the based character doesn't have
		EnhancedInputComponent->BindAction(PowerUpAction, ETriggerEvent::Started, this, &AChromarion::PowerUp);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component!"), *GetNameSafe(this));
	}
}


void AChromarion::LimitVelocity()
{
	if (CurrentColor == EColorType::Green)
	{
		FVector Velocity = GetCharacterMovement()->Velocity;
		Velocity.Y = FMath::Clamp(Velocity.Y, -500.f, 500.f);
		GetCharacterMovement()->Velocity = Velocity;
	}
}

EColorType AChromarion::GetCurrentColor()
{
	return CurrentColor;
}

void AChromarion::SetCurrentColor(EColorType newColor)
{
	if (newColor != EColorType::Uncolored && CurrentColor == newColor)
    {
		SetPowerLimit(0);
        return;
    }

	CurrentColor = newColor;
	// Vérifier que le SkeletalMesh existe
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		FString ColorName;

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

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Color : %s"), *ColorName));

		FString MainMaterialPath = FString::Printf(TEXT("Material'/Game/Characters/Chromarion/Materials/%s/M_Main.M_Main'"), *ColorName);
		UMaterialInterface* Main = LoadObject<UMaterialInterface>(nullptr, *MainMaterialPath);

		FString SecondaryMaterialPath = FString::Printf(TEXT("Material'/Game/Characters/Chromarion/Materials/%s/M_Secondary.M_Secondary'"), *ColorName);
		UMaterialInterface* Secondary = LoadObject<UMaterialInterface>(nullptr, *SecondaryMaterialPath);

		if (Main && Secondary )
		{
			// Appliquer le matériau au personnage (index 0 = matériau principal)
			MeshComp->SetMaterial(0, Main);
			MeshComp->SetMaterial(2, Secondary);
		}
		else
		{
			if (!Main)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("No material found at path : %s"), *MainMaterialPath));
			}
			if (!Secondary)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("No material found at path : %s"), *SecondaryMaterialPath));
			}
		}

		SetPowerUpWidget(static_cast<int32>(newColor));

		ResetPowerEffect();
	}
}

void AChromarion::SetPowerUpWidgetLimit(int32 NewPowerLimit)
{
	if (PowerUpSwitcher)
	{
		if (auto Widget = Cast<UPowerUpWidget>(PowerUpSwitcher->GetActiveWidget()))
		{
			Widget->SetPowerLimit(NewPowerLimit);
		}
	}
}

void AChromarion::SetPowerUpWidget(int32 PowerUp)
{
	if (PowerUpSwitcher)
	{
		PowerUpSwitcher->SetActiveWidgetIndex(PowerUp);
	}
}

void AChromarion::Move(const FInputActionValue& Value)
{
	Super::Move(Value);

	LastMovementInput = Value.Get<FVector2D>();

	UpdateShrinkWidgetPosition();
}

void AChromarion::PowerUp(const FInputActionValue& Value)
{
	switch (CurrentColor)
	{
	case EColorType::Red:
		// jump
		if (bIsOnGround)
		{
			GetCharacterMovement()->JumpZVelocity = 1200.f;
			bIsPowerActive = true;

			StartJump();
			PowerUsage();

			// Ajouter l'effet du jump
			if (JumpEffectComponent)
			{
				JumpEffectComponent->Activate();

				FTimerHandle TrailTimerHandle;
				GetWorldTimerManager().SetTimer(TrailTimerHandle, [this]() {JumpEffectComponent->Deactivate(); }, 0.5f, false);
			}

			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(TimerHandle, [this]()
			{
				if (PowerLimit == JumpLimitUsage)
				{
					ResetColor();
				}
			}, 1.0f, false);
		}
		break;
	case EColorType::Yellow:
		// dash
		if (bCanDash)
		{
			if (PowerLimit == (DashLimitUsage - 1))
			{
				FTimerHandle TimerHandle;
				GetWorldTimerManager().SetTimer(TimerHandle, [this]() { ResetColor(); }, 0.5f, false);
			}

			PowerUsage();

			FVector2D MoveInput = FVector2D(LastMovementInput.X, LastMovementInput.Y).GetSafeNormal();

			// Jouer le son de dash
			if (DashShoutSound && DashSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DashSound, GetActorLocation());
				UGameplayStatics::PlaySoundAtLocation(this, DashShoutSound, GetActorLocation());
			}

			// Ajouter l'effet du dash
			if (DashTrailComponent)
			{
				DashTrailComponent->Activate();

				FTimerHandle TrailTimerHandle;
				GetWorldTimerManager().SetTimer(TrailTimerHandle, [this](){DashTrailComponent->Deactivate();}, 0.5f, false);
			}
			
			// Si le joueur n'appuie sur aucune direction, on dash vers l'avant
			FVector DashDirection;
			if (!MoveInput.IsNearlyZero())
			{
				DashDirection = FVector(0.0f, MoveInput.X, MoveInput.Y);
			}
			else
			{
				DashDirection = FVector(0.0f, 1.0f, 0.0f);
			}

			FVector DashVelocity = DashDirection * 1200.f;
			LaunchCharacter(DashVelocity, true, true);

			bCanDash = false;
			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(TimerHandle, [this]() { bCanDash = true; }, 1.f, false);
		}
		break;
	case EColorType::Blue:
		// speed
		if (bCanSpeed)
		{
			if (PowerLimit == (SpeedLimitUsage - 1))
			{
				FTimerHandle TimerHandle;
				GetWorldTimerManager().SetTimer(TimerHandle, [this]() { ResetColor(); }, SpeedTime+0.1f, false);
			}

			PowerUsage();

			// Jouer le son du speed boost
			if (SpeedPowerUpSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, SpeedPowerUpSound, GetActorLocation());
			}

			// Ajouter l'effet de la speed
			if (SpeedTrailComponent)
			{
				SpeedTrailComponent->Activate();

				FTimerHandle TrailTimerHandle;
				GetWorldTimerManager().SetTimer(TrailTimerHandle, [this]() {SpeedTrailComponent->Deactivate(); }, 1.5f, false);
			}

			GetCharacterMovement()->MaxWalkSpeed = 1200.f;
			GetCharacterMovement()->MaxAcceleration = 10000.f;

			bCanSpeed = false;

			SprintTimeWidget->SetVisibility(ESlateVisibility::Visible);
			SprintTimeWidget->StartTimer();

			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(TimerHandle, [this]()
			{
				SprintTimeWidget->SetVisibility(ESlateVisibility::Hidden);
				bCanSpeed = true;
				GetCharacterMovement()->MaxWalkSpeed = 500.f;
				GetCharacterMovement()->MaxAcceleration = 1500.f;
			}, SpeedTime, false);
		}
		break;
	case EColorType::Orange:
		if (bIsPowerActive)
		{
			// check fi there is nothing above the character
			FHitResult Hit;
			FVector Start = GetActorLocation();
			FVector End = Start + FVector(0.f, 0.f, 120.f);
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(this);
			if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, CollisionParams)) return;
		}

		// scale
		if (PowerLimit >= ScaleLimitUsage)
		{
			ResetColor();
			return;
		}
		if (PowerLimit == ScaleLimitUsage - 1)
		{
			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(TimerHandle, [this]() { ResetColor(); }, 0.4f, false);
		}
		PowerUsage();

		if (bIsPowerActive)
		{

			ScaleEffectTimeline.ReverseFromEnd();
			GetCharacterMovement()->JumpZVelocity = 800.f;
			ShrinkWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			ScaleEffectTimeline.PlayFromStart();
			GetCharacterMovement()->JumpZVelocity = 600.f;
			ShrinkWidget->SetVisibility(ESlateVisibility::Visible);
		}
		bIsPowerActive = !bIsPowerActive;
		break;
	case EColorType::Green:
		// gravity
		if (PowerLimit == (GravityLimitUsage - 1))
		{
			FTimerHandle TimerHandle;  // Première variable Timer
			GetWorldTimerManager().SetTimer(TimerHandle, [this]() { ResetColor(); }, 0.5f, false);
		}

		PowerUsage();
		// Ajouter l'effet de la gravité
		if (GravityEffectComponent)
		{
			GravityEffectComponent->Activate();

			FTimerHandle TrailTimerHandle;
			GetWorldTimerManager().SetTimer(TrailTimerHandle, [this]() {GravityEffectComponent->Deactivate(); }, 0.5f, false);
		}

		if (bIsPowerActive)
		{
			GetCharacterMovement()->SetGravityDirection(FVector(0.f, 0.f, -1.f));
			FTimerHandle TimerHandle2;
			GetWorldTimerManager().SetTimer(TimerHandle2, [this]() { LimitVelocity(); }, 0.01f, false);
		}
		else
		{
			GetCharacterMovement()->SetGravityDirection(FVector(0.f, 0.f, 1.f));
			FTimerHandle TimerHandle2;
			GetWorldTimerManager().SetTimer(TimerHandle2, [this]() { LimitVelocity(); }, 0.01f, false);
		}

		bIsPowerActive = !bIsPowerActive;
	case EColorType::Purple:
		break;
	default:
		break;
	}
}

void AChromarion::ResetGravity()
{
	GetCharacterMovement()->SetGravityDirection(FVector(0.f, 0.f, -1.f));
}

void AChromarion::StartJump()
{
	if (bIsOnGround && bCanJump)
	{
		bIsOnGround = false;

		if (CurrentColor == EColorType::Red && bIsPowerActive)
		{
			UGameplayStatics::PlaySoundAtLocation(this, LongJumpSound, GetActorLocation());
		}
		else
		{
			// Vérifier si on peut jouer le son de saut
			USoundBase* JumpSound = FMath::RandBool() ? JumpSoundOne : JumpSoundTwo;
			UGameplayStatics::PlaySoundAtLocation(this, JumpSound, GetActorLocation());
		}
	}

	Super::StartJump();
}

void AChromarion::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (!bIsOnGround) bIsOnGround = true;

	if (CurrentColor == EColorType::Red && bIsPowerActive)
	{
		GetCharacterMovement()->JumpZVelocity = 800.f;
		bIsPowerActive = false;
	}

	UGameplayStatics::PlaySoundAtLocation(this, LandSound, GetActorLocation());
}

void AChromarion::ResetPowerEffect()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->JumpZVelocity = 800.f;
		GetCharacterMovement()->MaxWalkSpeed = 500.f;
		GetCharacterMovement()->SetGravityDirection(FVector(0.f, 0.f, -1.f));
	}
	if (GetMesh())
	{
		GetCapsuleComponent()->SetWorldScale3D(FVector(1.f));
	}
	bIsPowerActive = false;
	SetPowerLimit(0);
}

void AChromarion::ResetColor()
{
	CurrentColor = EColorType::Uncolored;
	SetCurrentColor(EColorType::Uncolored);
	ResetPowerEffect();
}

void AChromarion::SetPowerLimit(int32 NewPowerLimit)
{
	PowerLimit = NewPowerLimit;
	SetPowerUpWidgetLimit(PowerLimitArray[static_cast<int>(CurrentColor)] - NewPowerLimit);
}

void AChromarion::PowerUsage()
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ForceFeedbackEffect)
		{
			PC->ClientPlayForceFeedback(ForceFeedbackEffect);
		}
	}

	PowerLimit++;
	SetPowerUpWidgetLimit(PowerLimitArray[static_cast<int>(CurrentColor)] - PowerLimit);
	// widget for the -1 power up
	if (PowerUpSwitcher)
	{
		if (auto Widget = Cast<UPowerUpWidget>(PowerUpSwitcher->GetActiveWidget()))
		{
			Widget->PowerUsage();
		}
	}
}

void AChromarion::BeginPlay()
{
	Super::BeginPlay();

	if (ScaleEffectCurve)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("HandleScaling"));
		ScaleEffectTimeline.AddInterpFloat(ScaleEffectCurve, TimelineProgress);
		ScaleEffectTimeline.SetLooping(false);
	}

	if (PowerUpWidgetClass)
	{
		PowerUpWidget = CreateWidget<UUserWidget>(GetWorld(), PowerUpWidgetClass);
		PowerUpSwitcher = Cast<UWidgetSwitcher>(PowerUpWidget->GetWidgetFromName(TEXT("PowerUpSwitcher")));
		PowerUpWidget->AddToViewport();
	}

	if (SprintTimeWidgetClass)
	{
		SprintTimeWidget = CreateWidget<USprintTime>(GetWorld(), SprintTimeWidgetClass);
		SprintTimeWidgetComponent->SetWidget(SprintTimeWidget);
		SprintTimeWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (ShrinkPositionWidgetClass)
	{
		ShrinkWidget = CreateWidget<UUserWidget>(GetWorld(), ShrinkPositionWidgetClass);
		ShrinkPositionWidgetComponent->SetWidget(ShrinkWidget);
		ShrinkWidget->SetVisibility(ESlateVisibility::Hidden);
		UpdateShrinkWidgetPosition();
	}
	
	ResetColor();
	RespawnLocation = GetActorLocation();
}

void AChromarion::DeathCharacter(){

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathAnimation)
	{
		GetCharacterMovement()->DisableMovement();
		
		AnimInstance->Montage_Play(DeathAnimation);

		// Define a callback for the animation's end
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AChromarion::OnDeathAnimationEnded);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, DeathAnimation);
	}
	// On joue le son de mort
	USoundBase* DeathSound = FMath::RandBool() ? DeathSound1 : DeathSound2;
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation()); // 

	if (auto GM = Cast<AColorLeakGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GM->nbDeathChromarion++;
	}
}

void AChromarion::OnDeathAnimationEnded(UAnimMontage* Montage, bool bInterrupted){

	if(Montage == DeathAnimation){

		APlayerController* PC = Cast<APlayerController>(GetController());

		if(PC){
			AColorLeakGameMode* GM = Cast<AColorLeakGameMode>(GetWorld()->GetAuthGameMode());

			if(GM && RespawnClass){
				FName SpawnTag = "Chromarion";
				GM->RespawnPlayer(PC, RespawnClass, SpawnTag);

				GetCharacterMovement()->SetMovementMode(MOVE_Walking);

				PowerUpWidget->RemoveFromParent();

			}
		}
	}

}

void AChromarion::UpdateShrinkWidgetPosition()
// TODO : show widget if under something
{
	if (bIsPowerActive && this->GetCurrentColor() == EColorType::Orange)
	{
		if (ShrinkPositionWidgetComponent && ShrinkWidget)
		{
			// Set widget on top of Chromarion (Z=200) and over ground elements (X=-100)
			FVector NewWidgetLocation = GetActorLocation() + FVector(-220.f, 0.f, 200.f);
			ShrinkPositionWidgetComponent->SetWorldLocation(NewWidgetLocation);

			// Rotate widget in front of Camera
			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			if (PlayerController)
			{
				FVector ToCamera = PlayerController->PlayerCameraManager->GetCameraLocation() - NewWidgetLocation;
				ToCamera.Z = 0.f; 
				ToCamera.Normalize();
				
				ShrinkPositionWidgetComponent->SetWorldRotation(ToCamera.Rotation());
			}
		}
	}
}




// Inputs
/////////////////////////////////////////////////////////////////
