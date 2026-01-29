// Fill out your copyright notice in the Description page of Project Settings.


#include "ColorLeak_Level.h"

void AColorLeak_Level::BeginPlay()
{
    Super::BeginPlay();

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("C++ there !"));

    if (!Acryl || !Chromarion)
    {
        UE_LOG(LogTemp, Error, TEXT("No character to spawn"));
        return;
    }

    TArray<FInputDeviceId> ConnectedDevices;
    UInputDeviceLibrary::GetAllConnectedInputDevices(ConnectedDevices);

    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

    if (PlayerStarts.Num() <= 0)
    {
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No player starts found in the level"));
        return;
    }

    if (ConnectedDevices.Num() >= 1)
    {
        if (auto PlayerStart = Cast<APlayerStart>(PlayerStarts[0]))
        {
            if (GEngine)
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Acryl there !"));

            APawn* Pawn = GetWorld()->SpawnActor<APawn>(Acryl, PlayerStart->GetActorLocation(), PlayerStart->GetActorRotation());

            APlayerController* PlayerController = nullptr;
            if (auto ExistingController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
            {
                PlayerController = ExistingController;
                PlayerController->Possess(Pawn);
            }
            else
            {
                PlayerController = UGameplayStatics::CreatePlayer(GetWorld(), 0);
                PlayerController->Possess(Pawn);
            }
        }
        if (auto PlayerStart = Cast<APlayerStart>(PlayerStarts[1]))
        {
            if (GEngine)
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Chromarion there !"));

            APawn* Pawn = GetWorld()->SpawnActor<APawn>(Chromarion, PlayerStart->GetActorLocation(), PlayerStart->GetActorRotation());

            APlayerController* PlayerController = nullptr;
            if (auto ExistingController = UGameplayStatics::GetPlayerController(GetWorld(), 1))
            {
                PlayerController = ExistingController;
                PlayerController->Possess(Pawn);
            }
            else
            {
                PlayerController = UGameplayStatics::CreatePlayer(GetWorld(), 1);
                PlayerController->Possess(Pawn);
            }
        }
    }
}