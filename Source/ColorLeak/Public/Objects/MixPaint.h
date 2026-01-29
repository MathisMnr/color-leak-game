// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/Paint.h"
#include "MixPaint.generated.h"

/**
 * 
 */
UCLASS()
class COLORLEAK_API AMixPaint : public APaint
{
	GENERATED_BODY()

public:

	AMixPaint();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mix, meta = (AllowPrivateAccess = "true"))
	bool endMix = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mix, meta = (AllowPrivateAccess = "true"))
	EColorType firstColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mix, meta = (AllowPrivateAccess = "true"))
	EColorType secondColor;

	virtual void BeginPlay() override;

	void mixColor();

	FString convertColor(EColorType color);

	void changeColor(FString colorName);

	FLinearColor ConvertColorTypeToLinearColor(EColorType ColorType);



protected:

	UPROPERTY(EditAnywhere,BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* WidgetComponentColors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> WidgetColorsClass;	

	UPROPERTY(BlueprintReadOnly)
	class UUserWidget* WidgetColors;

	void ChangeWidgetImageColor(FName ImageName, FLinearColor NewColor);

private:
	bool bIsRelocated = false;
};
