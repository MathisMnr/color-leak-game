// Fill out your copyright notice in the Description page of Project Settings.


#include "ColorLeakSaveGame.h"

#include "ColorLeakGameInstance.h"

UColorLeakSaveGame::UColorLeakSaveGame()
{
	UE_LOG(LogTemp, Display, TEXT("UColorLeakSaveGame size: %d"), static_cast<int32>(ELevels::Purple_2) + 1);
	BestTimeArray.Init(INFINITY, static_cast<int32>(ELevels::Purple_2) + 1);
}
