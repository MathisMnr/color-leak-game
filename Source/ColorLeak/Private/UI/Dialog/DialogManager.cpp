// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Dialog/DialogManager.h"

// Define the static delegate
FOnDialogTriggerEvent UDialogManager::OnDialogTriggered;

void UDialogManager::TriggerEvent(FName TriggerName)
{
	// Broadcast the event to all listeners
	OnDialogTriggered.Broadcast(TriggerName);
}
