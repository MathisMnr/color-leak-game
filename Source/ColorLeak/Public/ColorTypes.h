#pragma once

#include "CoreMinimal.h"

// Define an enum for colors
UENUM(BlueprintType)
enum class EColorType : uint8
{
    Uncolored UMETA(DisplayName = "Uncolored"),
    Red       UMETA(DisplayName = "Red"),
    Blue      UMETA(DisplayName = "Blue"),
    Yellow    UMETA(DisplayName = "Yellow"),
    Green     UMETA(DisplayName = "Green"),
    Orange    UMETA(DisplayName = "Orange"),
    Purple    UMETA(DisplayName = "Purple")
};

FColor FromColor(EColorType Color);
