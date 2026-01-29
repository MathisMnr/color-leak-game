// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EaselUI.generated.h"

/**
 * 
 */
UCLASS()
class COLORLEAK_API UEaselUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* LevelText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn))
	FText Message;

	// image
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* LevelImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn))
	FSlateBrush ImageBrush;

	virtual void NativeConstruct() override;

	void SetShow(bool bShow);

	void SetImageBrush(FSlateBrush Brush);

	void SetText(FText Text);
};
