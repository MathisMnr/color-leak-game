// Copyright Epic Games, Inc. All Rights Reserved.

#include "ColorLeakGameMode.h"
#include "ColorLeakCharacter.h"
#include "ColorLeakGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "UI/SpeedRun/SpeedRunTimeWidget.h"
#include "UObject/ConstructorHelpers.h"

AColorLeakGameMode::AColorLeakGameMode()
{
}

void AColorLeakGameMode::RespawnPlayer(AController* Controller, TSubclassOf<APawn> CharacterClass, FName SpawnTag)
{
	if (Controller && CharacterClass)
	{
		FVector SpawnLocation = FVector::ZeroVector;
		FRotator SpawnRotation = FRotator::ZeroRotator;

		if (AColorLeakCharacter* Character = Cast<AColorLeakCharacter>(Controller->GetPawn()))
		{
			if (!Character->RespawnLocation.IsZero())
			{
				SpawnLocation = Character->RespawnLocation;
				SpawnRotation = FRotator(0.f, 90.f, 0.f);
				/*
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("CheckPoint X: %f "),SpawnLocation.X) );
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("CheckPoint Y: %f "),SpawnLocation.Y) );
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("CheckPoint Z: %f "),SpawnLocation.Z) );
				*/
			}
		}

		if (SpawnLocation.IsZero())
		{
			TArray<AActor*> SpawnPoints;
			UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), SpawnPoints);

			AActor* ChosenSpawn = nullptr;
			for (AActor* Spawn : SpawnPoints)
			{
				if (Spawn->ActorHasTag(SpawnTag))
				{
					ChosenSpawn = Spawn;
					break;
				}
			}

			if (ChosenSpawn)
			{
				SpawnLocation = ChosenSpawn->GetActorLocation();
				SpawnRotation = ChosenSpawn->GetActorRotation();
			}
		}

		if (!SpawnLocation.IsZero())
		{
			dead = true;

			if (APawn* OldPawn = Controller->GetPawn())
			{
				OldPawn->Destroy();
			}

			FTimerHandle RespawnTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, FTimerDelegate::CreateLambda(
				                                       [this, Controller, CharacterClass, SpawnLocation, SpawnRotation
				                                       ]()
				                                       {
					                                       FActorSpawnParameters SpawnParams;
					                                       SpawnParams.SpawnCollisionHandlingOverride =
						                                       ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
					                                       APawn* NewPawn = GetWorld()->SpawnActor<APawn>(
						                                       CharacterClass, SpawnLocation, SpawnRotation,
						                                       SpawnParams);
					                                       /*
					                                       GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("CheckPoint X: %f "),NewPawn->GetActorLocation().X) );
					                                       GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("CheckPoint Y: %f "),NewPawn->GetActorLocation().Y) );
					                                       GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("CheckPoint Z: %f "),NewPawn->GetActorLocation().Z) );
					                                       */
					                                       if (NewPawn)
					                                       {
						                                       Controller->Possess(NewPawn);
						                                       respawn = true;
						                                       dead = false;
					                                       }
					                                       else
					                                       {
						                                       GEngine->AddOnScreenDebugMessage(
							                                       -1, 5.f, FColor::Red,
							                                       FString::Printf(TEXT("No NewPawn")));
					                                       }
				                                       }), 1.0f, false);
		}
	}
}

void AColorLeakGameMode::StartDialog(FName DialogName)
{
	if (auto GI = Cast<UColorLeakGameInstance>(GetGameInstance()))
	{
		if (GI->bIsSpeedRun) return;
	}

	if (!DialogWidget || bIsDialogActive) return;

	if (!DialogWidget->HasDialog(DialogName)) return;

	this->SetPlayerMovementEnabled(false);

	bIsDialogActive = true;
	DialogWidget->AddToViewport();
	DialogWidget->LoadDialog(DialogName);
}

void AColorLeakGameMode::AdvanceDialog()
{
	if (bIsDialogActive && DialogWidget)
	{
		DialogWidget->AdvanceDialog();
	}
}

void AColorLeakGameMode::SetDialogActive(bool bIsActive)
{
	this->bIsDialogActive = bIsActive;
}

void AColorLeakGameMode::SetPlayerMovementEnabled(bool bEnable)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC)
		{
			PC->SetIgnoreMoveInput(!bEnable);
			PC->SetIgnoreLookInput(!bEnable);

			if (auto Player = Cast<AColorLeakCharacter>(PC->GetPawn()))
			{
				Player->bCanJump = bEnable;
			}
		}
	}

	/*if (auto PC = GetWorld()->GetFirstPlayerController())
	{
	    PC->SetPause(!bEnable);
	}*/
}

void AColorLeakGameMode::AddIGT()
{
	if (auto GI = Cast<UColorLeakGameInstance>(GetGameInstance()))
	{
		if (SpeedRunTimeWidget)
		{
			SpeedRunTimeWidget->StopTime();
			GI->AddIGT(SpeedRunTimeWidget->GetActualTime());
			GI->nbDeathAcryl += nbDeathAcryl;
			GI->nbDeathChromarion += nbDeathChromarion;
		}
	}
}

void AColorLeakGameMode::StartSpeedRun()
{
	if (auto GI = Cast<UColorLeakGameInstance>(GetGameInstance()))
	{
		if (GI->bIsSpeedRun && SpeedRunTimeWidgetClass)
		{
			//SpeedRunTimeWidget->SetVisibility(ESlateVisibility::Visible);
			SpeedRunTimeWidget->SetRenderOpacity(true);
		}
	}
}

float AColorLeakGameMode::GetFinishTime()
{
	// log time
	UE_LOG(LogTemp, Display, TEXT("GetFinishTime : %f"), SpeedRunTimeWidget->GetActualTime());
	return SpeedRunTimeWidget->GetActualTime();
}

void AColorLeakGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (DialogWidgetClass)
	{
		DialogWidget = CreateWidget<UDialogWidget>(GetWorld(), DialogWidgetClass);
	}

	if (SpeedRunTimeWidgetClass)
	{
		SpeedRunTimeWidget = CreateWidget<USpeedRunTimeWidget>(GetWorld(), SpeedRunTimeWidgetClass);
		SpeedRunTimeWidget->AddToViewport();
		//SpeedRunTimeWidget->SetVisibility(ESlateVisibility::Hidden);
		SpeedRunTimeWidget->SetRenderOpacity(false);
	}
}
