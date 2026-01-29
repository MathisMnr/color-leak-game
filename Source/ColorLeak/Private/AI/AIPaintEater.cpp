// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIPaintEater.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ProgressBar.h"
#include "Acryl.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAIPaintEater::AAIPaintEater()
{
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
	
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Color = EColorType::Uncolored;
	DetectionDistance = 400.0f;
	EatingRange = 180.0f;
	EatingTime = 3.0f;
	EatingTimeCounter = 0.f; 

	EatingDetectionBox = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	EatingDetectionBox->SetupAttachment(GetMesh());
	EatingDetectionBox->SetWorldScale3D(FVector(1, EatingRange/100, 1));
}

// Called when the game starts or when spawned
void AAIPaintEater::BeginPlay()
{
	Super::BeginPlay();
	//Disable X movement
	
	//GetCharacterMovement()->bConstrainToPlane = true;
	//GetCharacterMovement()->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::X);

	bUseControllerRotationYaw = false;		// Block rotations
		
	UpdateColor();

	this->OnActorBeginOverlap.AddDynamic(this, &AAIPaintEater::OnOverlap);
	this->OnActorEndOverlap.AddDynamic(this, &AAIPaintEater::OnEndOverlap);	
	bIsEating = false;

	if (ProgressBarWidgetClass)
	{
		ProgressBarWidget = CreateWidget<UUserWidget>(GetWorld(), ProgressBarWidgetClass);
		if (ProgressBarWidget)
		{
			ProgressBarWidget->AddToViewport();
			ProgressBarWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	
	UpdateProgressBarColor();
	UpdateWidgetPosition();

	SetIsChasing(true);
}

// Called to bind functionality to input
void AAIPaintEater::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAIPaintEater::UpdateColor()
{
	FString ColorName;

	switch (Color)
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
		// Apply material to AI Paint Eater
		GetMesh()->SetMaterial(0, Main);
		GetMesh()->SetMaterial(2, Secondary);
	}
}

void AAIPaintEater::OnOverlap(AActor* MyActor, AActor* OtherActor)
{
	if (auto Acryl = Cast<AAcryl>(OtherActor))
	{
		// we start widget update position when we enter the trigger
		if (!bIsLaunched)
		{
			bIsLaunched = true;
			GetWorld()->GetTimerManager().SetTimer(WigdetPositionTimerHandle, FTimerDelegate::CreateLambda([this]()
			{
				UpdateWidgetPosition();
			}), 0.2f, true);
		}
		
		if (!bIsEating && Acryl->GetColorBrush("Socket_Brush") == this->Color && bIsChasing)
		{
			if (ProgressBarWidget)
			{
				ProgressBarWidget->SetVisibility(ESlateVisibility::Visible);
			}
				
			GetWorld()->GetTimerManager().SetTimer(EatingTimerHandle, FTimerDelegate::CreateLambda([this, Acryl]()
			{
				bIsEating = true;
				EatingTimeCounter += 0.1f;
				UpdateProgressBar(Acryl);
			}), 0.1f, true);
		}
		if (!bIsChasing) { ProgressBarWidget->SetVisibility(ESlateVisibility::Hidden); }
	}
}

void AAIPaintEater::OnEndOverlap(AActor* MyActor, AActor* OtherActor)
{
	if (auto Acryl = Cast<AAcryl>(OtherActor))
	{
		EatingTimeCounter = 0.f;  
		if (ProgressBarWidget)
		{
			UpdateProgressBar(Acryl);
			ProgressBarWidget->SetVisibility(ESlateVisibility::Hidden);  
		}
		bIsEating = false;
		GetWorld()->GetTimerManager().ClearTimer(EatingTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(LickSoundTimerHandle);
	}
}

void AAIPaintEater::UpdateProgressBar(AAcryl* Acryl)
{
	if (bIsEating)
	{
		int CurrentSecond = FMath::FloorToInt(EatingTimeCounter);
        
		// Vérifie si on est dans une nouvelle seconde
		if (LastPlayedLickSoundTime == -1 || CurrentSecond > LastPlayedLickSoundTime)
		{
			LastPlayedLickSoundTime = CurrentSecond;

			// Joue le LickSound
			if (AudioComponent && LickSound)
			{
				AudioComponent->SetSound(LickSound);
				AudioComponent->Play();
			}
		}
		// If AI have finished eating Acryl's brush color
		if (EatingTimeCounter >= EatingTime)
		{
			// Reset the eating time counter
			EatingTimeCounter = 0.f;  
			// Eat the color
			if (Acryl)
			{
				Acryl->ClearBrush();
			}
			// IA finished eating
			bIsEating = false;  
			// Play eating blurp sound
			if (AudioComponent && BlurpSound)
			{
				/*
				int RandomBlurp = FMath::RandRange(1, 4);
				FString BlurpPath = FString::Printf(TEXT("Material'/Game/Characters/AI/SFX/Blurp%d.Blurp%d'"), RandomBlurp);
				BlurpSound = LoadObject<USoundWave>(nullptr, *BlurpPath);
				*/
				AudioComponent->SetSound(BlurpSound);
				AudioComponent->Play();
				LastPlayedLickSoundTime = -1; // Reset the last played lick sound time
			}
			// Hide progess bar
			if (ProgressBarWidget)
			{
				ProgressBarWidget->SetVisibility(ESlateVisibility::Hidden);
			}
			// Stop timers
			GetWorld()->GetTimerManager().ClearTimer(LickSoundTimerHandle);
			GetWorld()->GetTimerManager().ClearTimer(EatingTimerHandle);
		}

		// Update progression bar
		if (ProgressBarWidget)
		{
			UProgressBar* ProgressBar = Cast<UProgressBar>(ProgressBarWidget->GetWidgetFromName(TEXT("EatProgressBar")));
			if (ProgressBar)
			{
				// Calculate progression bar percentage
				float Progress = EatingTimeCounter / EatingTime;
				ProgressBar->SetPercent(Progress);

				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Progress : %f"), Progress));
			}
		}
	}
}

void AAIPaintEater::UpdateWidgetPosition()
{
	if (ProgressBarWidget)
	{
		USkeletalMeshComponent* MeshComp = GetMesh();
		if (MeshComp)
		{
			// set widget over head
			FVector HeadPosition = MeshComp->GetSocketLocation("head");
			FVector Offset(0, -50.f, 100.f);  
			HeadPosition += Offset;

			FVector2D ScreenPosition;
			if (UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), HeadPosition, ScreenPosition))
			{
				ProgressBarWidget->SetPositionInViewport(ScreenPosition);
			}
		}
	}
}

void AAIPaintEater::UpdateProgressBarColor()
{
	if (ProgressBarWidget)
	{
		UProgressBar* ProgressBar = Cast<UProgressBar>(ProgressBarWidget->GetWidgetFromName(TEXT("EatProgressBar")));
		if (ProgressBar)
		{
			FLinearColor NewColor;

			switch (Color)
			{
			case EColorType::Red:
				NewColor = FLinearColor(0.998f, 0.002f, 0.0f, 1.0f);
				break;
			case EColorType::Yellow:
				NewColor =FLinearColor(1.f, 1.f, 0.0f, 1.0f);
				break;
			case EColorType::Blue:
				NewColor = FLinearColor(0.0f, 0.002f, 1.0f, 1.0f);;
				break;
			case EColorType::Green:
				NewColor = FLinearColor(0.0f, 1.0, 0.0f, 1.0f);;
				break;
			case EColorType::Orange:
				NewColor = FLinearColor(1.0f, 0.234f, 0.0f, 1.0f); 
				break;
			case EColorType::Purple:
				NewColor = FLinearColor(0.248656f, 0.000912f, 0.45625f, 1.0f); 
				break;
			case EColorType::Uncolored: 
				NewColor = FLinearColor(0.494118f, 0.494118f, 0.494118f, 1.0f); 
				break;
			default:
				NewColor = FLinearColor(0.494118f, 0.494118f, 0.494118f, 1.0f);
				break;
			}

			// Appliquer la couleur au remplissage de la ProgressBar
			ProgressBar->SetFillColorAndOpacity(NewColor);
		}
	}
}

void AAIPaintEater::SetIsChasing(bool bNewState)
{
	bIsChasing = bNewState; 
	
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
		if (BlackboardComp)
		{
			BlackboardComp->SetValueAsBool("IsChasing", bIsChasing);
		}
	}
}


void AAIPaintEater::SetGravity(bool bNewGravityState)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->GravityScale = bNewGravityState ? 1.0f : 0.0f;
	}
}