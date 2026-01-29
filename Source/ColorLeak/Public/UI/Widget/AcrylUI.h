// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AcrylUI.generated.h"

/**
 * 
 */
UCLASS()
class COLORLEAK_API UAcrylUI : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void UpdateAcrylUI(const FString& NewColor);
};
