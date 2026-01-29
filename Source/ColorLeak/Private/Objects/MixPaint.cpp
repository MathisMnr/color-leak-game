// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/MixPaint.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"

AMixPaint::AMixPaint(){

    WidgetComponentColors = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponentColors"));
	WidgetComponentColors->SetupAttachment(RootComponent);
    WidgetComponentColors->SetDrawSize(FVector2D(669.f, 500.f));
}


void AMixPaint::BeginPlay(){
    Super::BeginPlay();

    firstColor = EColorType::Uncolored;
    secondColor = EColorType::Uncolored;

    if (WidgetColorsClass)
    {
        WidgetComponentColors->SetWidgetClass(WidgetColorsClass);
        WidgetComponentColors->SetRelativeLocation(FVector(0.0f, 0.0f, 56.0f));
        WidgetComponentColors->InitWidget();
    }
}

void AMixPaint::ChangeWidgetImageColor(FName ImageName, FLinearColor NewColor)
{
    if (!WidgetColors) return;

    UImage* TargetImage = Cast<UImage>(WidgetColors->GetWidgetFromName(ImageName));
    if (TargetImage)
    {
        TargetImage->SetColorAndOpacity(NewColor);
    }
}

void AMixPaint::changeColor(FString colorName){

    FString PaintMaterialPath = FString::Printf(TEXT("/Script/Engine.Material'/Game/Characters/Acryl/Brush/M_paint%s.M_paint%s'"), *colorName, *colorName);
    UMaterialInterface* PaintColor = LoadObject<UMaterialInterface>(nullptr, *PaintMaterialPath);

    if (PaintColor)
    {
        StaticMesh->SetMaterial(2, PaintColor);
    } else{
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Error Material"));
    }
}

void AMixPaint::mixColor(){

    WidgetColors = Cast<UUserWidget>(WidgetComponentColors->GetWidget());
    FLinearColor colorLinear = FLinearColor::White;

    if(firstColor != EColorType::Uncolored && !bIsRelocated){

        UStaticMesh* potPaint = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Assets/Decors/Pot/sceau.sceau"));
        if (potPaint)
        {
            StaticMesh->SetStaticMesh(potPaint);
            StaticMesh->SetWorldLocation(GetActorLocation()-FVector(35.0f, 34.0f, 38.0f));
            bIsRelocated = true;
            WidgetComponentColors->SetRelativeLocation(FVector(21.578616, 21.0, 80));

            FString colorName = convertColor(firstColor);
            changeColor(colorName);

            colorLinear = ConvertColorTypeToLinearColor(firstColor);

            if (WidgetColors)
            {
                ChangeWidgetImageColor("Paint1", colorLinear);
            }
        }
        
    }

    if(firstColor != EColorType::Uncolored && secondColor != EColorType::Uncolored){

        colorLinear = ConvertColorTypeToLinearColor(secondColor);

        if (WidgetColors)
        {
            ChangeWidgetImageColor("Paint2", colorLinear);
        }

        if(firstColor == EColorType::Yellow && secondColor == EColorType::Red || firstColor == EColorType::Red && secondColor == EColorType::Yellow){

            FString colorName = TEXT("Orange");
            changeColor(colorName);

            colorLinear = ConvertColorTypeToLinearColor(EColorType::Orange);

            if (WidgetColors)
            {
                ChangeWidgetImageColor("Paint3", colorLinear);
            }

        } else if(firstColor == EColorType::Red && secondColor == EColorType::Blue || firstColor == EColorType::Blue && secondColor == EColorType::Red){

            FString colorName = TEXT("Purple");
            changeColor(colorName);

            colorLinear = ConvertColorTypeToLinearColor(EColorType::Purple);

            if (WidgetColors)
            {
                ChangeWidgetImageColor("Paint3", colorLinear);
            }

        } else if(firstColor == EColorType::Yellow && secondColor == EColorType::Blue || firstColor == EColorType::Blue && secondColor == EColorType::Yellow){

            FString colorName = TEXT("Green");
            changeColor(colorName);

            colorLinear = ConvertColorTypeToLinearColor(EColorType::Green);

            if (WidgetColors)
            {
                ChangeWidgetImageColor("Paint3", colorLinear);
            }
            
        }

        endMix = true;
    }

}


FString AMixPaint::convertColor(EColorType color){

	switch (color)
	{
		case EColorType::Red:
			return TEXT("Red");
			break;
		case EColorType::Yellow:
			return TEXT("Yellow");
			break;
		case EColorType::Blue:
			return TEXT("Blue");
			break;
		case EColorType::Green:
			return TEXT("Green");
			break;
		case EColorType::Orange:
			return TEXT("Orange");
			break;
		case EColorType::Purple:
			return TEXT("Purple");
			break;
		default:
			return TEXT("UnColored");
			break;
	}
}

FLinearColor AMixPaint::ConvertColorTypeToLinearColor(EColorType ColorType)
{
    switch (ColorType)
    {
    case EColorType::Red:
        return FLinearColor::Red;
    case EColorType::Green:
        return FLinearColor::Green;
    case EColorType::Blue:
        return FLinearColor::Blue;
    case EColorType::Yellow:
        return FLinearColor::Yellow;
    case EColorType::Orange:
        return FLinearColor(1.0, 0.234, 0.0);
    case EColorType::Purple:
        return FLinearColor(0.248656, 0.000912, 0.45625);
    default:
        return FLinearColor::White;
    }
}
