// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DialogManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogTriggerEvent, FName, TriggerName);

/**
 * 
 */
UCLASS()
class COLORLEAK_API UDialogManager : public UObject
{
	GENERATED_BODY()

public:
	// Global delegate for dialog triggers
	static FOnDialogTriggerEvent OnDialogTriggered;

	// Function to trigger an event
	static void TriggerEvent(FName TriggerName);
};
