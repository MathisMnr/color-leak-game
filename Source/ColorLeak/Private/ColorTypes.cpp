#include "ColorTypes.h"

FColor FromColor(EColorType Color)
{
	switch (Color)
	{
	case EColorType::Red:
		return FColor::Red;
	case EColorType::Yellow:
		return FColor::Yellow;
	case EColorType::Blue:
		return FColor::Blue;
	case EColorType::Green:
		return FColor::Green;
	case EColorType::Orange:
		return FColor::Orange;
	case EColorType::Purple:
		return FColor::Purple;
	default:
		return FColor(255, 168, 46, 1.0);
	}
}
