// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Easel/EaselUI.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UEaselUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (LevelText && !Message.IsEmpty())
	{
		LevelText->SetText(Message);
	}

	if (LevelImage && ImageBrush.GetResourceObject())
	{
		LevelImage->SetBrush(ImageBrush);
	}
}

void UEaselUI::SetShow(bool bShow)
{
	SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UEaselUI::SetImageBrush(FSlateBrush Brush)
{
	if (LevelImage)
	{
		LevelImage->SetBrush(Brush);
	}
}

void UEaselUI::SetText(FText Text)
{
	if (LevelText)
	{
		LevelText->SetText(Text);
	}
}
