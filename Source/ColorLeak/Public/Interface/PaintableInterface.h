// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ColorTypes.h"
#include "PaintableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPaintableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COLORLEAK_API IPaintableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void SetCurrentColor(EColorType newColor);
};
