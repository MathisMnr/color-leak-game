// Fill out your copyright notice in the Description page of Project Settings.


#include "ColorLeakGameInstance.h"

FText UColorLeakGameInstance::GetLevelName(ELevels Level)
{
	switch (Level)
	{
	case ELevels::Red_1:
		return FText::FromString(TEXT("Le Super Saut"));
	case ELevels::Red_2:
		return FText::FromString(TEXT("Emboitement coloré"));
	case ELevels::Blue_1:
		return FText::FromString(TEXT("Frénésie Flottante"));
	case ELevels::Blue_2:
		return FText::FromString(TEXT("Cascade et saut de l'ange"));
	case ELevels::LongLevel_1:
		return FText::FromString(TEXT("Un crabe et de la poudre"));
	case ELevels::Yellow_1:
		return FText::FromString(TEXT("Propulsions vertigineuses"));
	case ELevels::Yellow_2:
		return FText::FromString(TEXT("Les 7 anneaux colorés"));
	case ELevels::Orange_1:
		return FText::FromString(TEXT("Pour les grands et les petits !"));
	case ELevels::Orange_2:
		return FText::FromString(TEXT("Tout est lié"));
	case ELevels::Green_1:
		return FText::FromString(TEXT("La tête à l'envers"));
	case ELevels::Green_2:
		return FText::FromString(TEXT("Une balade renversée"));
	case ELevels::Purple_1:
		return FText::FromString(TEXT("Reflet lumineux"));
	case ELevels::Purple_2:
		return FText::FromString(TEXT("Corridor étincelant"));
	default:
		return FText::FromString(TEXT("Unknow"));
	}
}

FName UColorLeakGameInstance::GetDevLevelName(ELevels Level)
{
	switch (Level)
	{
	case ELevels::Red_1:
		return FName("Red_1");
	case ELevels::Red_2:
		return FName("Red_2");
	case ELevels::Blue_1:
		return FName("Blue_1");
	case ELevels::Blue_2:
		return FName("Blue_2");
	case ELevels::LongLevel_1:
		return FName("LongLevel_1");
	case ELevels::Yellow_1:
		return FName("Yellow_1");
	case ELevels::Yellow_2:
		return FName("Yellow_2");
	case ELevels::Orange_1:
		return FName("Orange_1");
	case ELevels::Orange_2:
		return FName("Orange_2");
	case ELevels::Green_1:
		return FName("Green_1");
	case ELevels::Green_2:
		return FName("Green_2");
	case ELevels::Purple_1:
		return FName("Purple_1");
	case ELevels::Purple_2:
		return FName("Purple_2");
	default:
		return FName("Lobby");
	}
}

ELevels UColorLeakGameInstance::GetPreviousLevel(ELevels CurrentLevel)
{
	if (CurrentLevel == ELevels::Red_1)
	{
		return ELevels::Red_1;
	}

	return static_cast<ELevels>(static_cast<uint8>(CurrentLevel) - 1);
}

ELevels UColorLeakGameInstance::GetLevelByName(FName LevelName)
{
	if (LevelName == "Red_1")
	{
		return ELevels::Red_1;
	}
	else if (LevelName == "Red_2")
	{
		return ELevels::Red_2;
	}
	else if (LevelName == "Blue_1")
	{
		return ELevels::Blue_1;
	}
	else if (LevelName == "Blue_2")
	{
		return ELevels::Blue_2;
	}
	else if (LevelName == "LongLevel_1")
	{
		return ELevels::LongLevel_1;
	}
	else if (LevelName == "Yellow_1")
	{
		return ELevels::Yellow_1;
	}
	else if (LevelName == "Yellow_2")
	{
		return ELevels::Yellow_2;
	}
	else if (LevelName == "Orange_1")
	{
		return ELevels::Orange_1;
	}
	else if (LevelName == "Orange_2")
	{
		return ELevels::Orange_2;
	}
	else if (LevelName == "Green_1")
	{
		return ELevels::Green_1;
	}
	else if (LevelName == "Green_2")
	{
		return ELevels::Green_2;
	}
	else if (LevelName == "Purple_1")
	{
		return ELevels::Purple_1;
	}
	else if (LevelName == "Purple_2")
	{
		return ELevels::Purple_2;
	}

	return ELevels::Red_1; // Default case
}

void UColorLeakGameInstance::FinishLevel(ELevels Level)
{
	if (!FinishedLevels.Contains(Level)) FinishedLevels.Add(Level);
}

bool UColorLeakGameInstance::IsLevelFinished(ELevels Level)
{
	return FinishedLevels.Contains(Level);
}

TArray<ELevels> UColorLeakGameInstance::GetFinishedLevels()
{
	return FinishedLevels;
}
