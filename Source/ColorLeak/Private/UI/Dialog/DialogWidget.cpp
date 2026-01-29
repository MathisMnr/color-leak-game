// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Dialog/DialogWidget.h"

#include "ColorLeak/ColorLeakGameMode.h"
#include "Components/Border.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "UI/Dialog/DialogManager.h"
#include <Components/RichTextBlockDecorator.h>
#include "Components/RichTextBlock.h"
#include "Kismet/GameplayStatics.h"

void UDialogWidget::FixScrollToEnd()
{
	if (!DialogBorder) return;

	// Scroll tout en bas
	TextContainer->ScrollToEnd();

	// Encore besoin de répéter ?
	if (++ScrollFixCounter < 5)
	{
		// Relance dans 0.02 secondes (~1 frame)
		GetWorld()->GetTimerManager().SetTimer(ScrollFixTimerHandle, this, &UDialogWidget::FixScrollToEnd, 0.02f, false);
	}
	else
	{
		// Reset le compteur
		ScrollFixCounter = 0;
	}
}

void UDialogWidget::AddTextPlayer1(const FMessageData& Message)
{
	if (!TextContainer) return;

	UHorizontalBox* HBox = NewObject<UHorizontalBox>(this);
	URichTextBlock* TextBlock = NewObject<URichTextBlock>(this);
	UBorder* TextBackground = NewObject<UBorder>(this);

	if (!HBox || !TextBlock || !TextBackground) return;

	if (!Message.SpeakerName.IsEmpty())
	{
		P1_Name->SetText(Message.SpeakerName);
	}
	if (Message.SpeakerImage)
	{
		P1_Image->SetBrushFromTexture(Message.SpeakerImage);
	}

	// Couleur de fond (violet #7D17BBFF)
	TextBackground->SetBrushColor(FLinearColor(0.49f, 0.09f, 0.73f, 0.7f));
	TextBackground->SetPadding(FMargin(10.f));

	TextBlock->SetTextStyleSet(RichTextStylesheet);
	TextBlock->SetText(Message.DialogText);
	TextBlock->SetAutoWrapText(true);
	TextBlock->SetJustification(ETextJustify::Left);

	TextBackground->SetContent(TextBlock);
	HBox->AddChild(TextBackground);

	if (auto HSlot = Cast<UHorizontalBoxSlot>(TextBackground->Slot))
	{
		HSlot->SetSize(ESlateSizeRule::Fill);
		HSlot->SetPadding(FMargin(0, 5, 50, 5));
	}

	if (auto VSlot = Cast<UScrollBoxSlot>(HBox->Slot))
	{
		VSlot->SetHorizontalAlignment(HAlign_Left);
	}

	TextContainer->AddChild(HBox);
	SynchronizeProperties();
	TextContainer->RemoveFromParent();
	DialogBorder->AddChild(TextContainer);

	P1_Image->SetOpacity(1.0f);
	P1_Name->SetOpacity(1.0f);
	P1_Name->SetShadowColorAndOpacity(P1_Name->GetShadowColorAndOpacity().CopyWithNewOpacity(1.0f));

	P2_Image->SetOpacity(.6f);
	P2_Name->SetOpacity(.3f);
	P2_Name->SetShadowColorAndOpacity(P2_Name->GetShadowColorAndOpacity().CopyWithNewOpacity(0.2f));

	FixScrollToEnd();
}



void UDialogWidget::AddTextPlayer2(const FMessageData& Message)
{
	if (!TextContainer) return;

	UHorizontalBox* HBox = NewObject<UHorizontalBox>(this);
	URichTextBlock* TextBlock = NewObject<URichTextBlock>(this);
	UBorder* TextBackground = NewObject<UBorder>(this);

	if (!HBox || !TextBlock || !TextBackground) return;

	// Couleur de fond (bleu foncé #02449FFF)
	TextBackground->SetBrushColor(FLinearColor(0.01f, 0.27f, 0.62f, 0.7f));
	TextBackground->SetPadding(FMargin(10.f));

	TextBlock->SetTextStyleSet(RichTextStylesheet);
	TextBlock->SetText(Message.DialogText);
	TextBlock->SetAutoWrapText(true);
	TextBlock->SetJustification(ETextJustify::Right);

	TextBackground->SetContent(TextBlock);
	HBox->AddChild(TextBackground);

	if (!Message.SpeakerName.IsEmpty())
	{
		P2_Name->SetText(Message.SpeakerName);
	}
	if (Message.SpeakerImage)
	{
		P2_Image->SetBrushFromTexture(Message.SpeakerImage);
	}

	if (auto HSlot = Cast<UHorizontalBoxSlot>(TextBackground->Slot))
	{
		HSlot->SetSize(ESlateSizeRule::Fill);
		HSlot->SetPadding(FMargin(50, 5, 0, 5));
	}

	if (auto VSlot = Cast<UScrollBoxSlot>(HBox->Slot))
	{
		VSlot->SetHorizontalAlignment(HAlign_Right);
	}

	TextContainer->AddChild(HBox);
	SynchronizeProperties();
	TextContainer->RemoveFromParent();
	DialogBorder->AddChild(TextContainer);

	P1_Image->SetOpacity(.6f);
	P1_Name->SetOpacity(.3f);
	P1_Name->SetShadowColorAndOpacity(P1_Name->GetShadowColorAndOpacity().CopyWithNewOpacity(0.2f));

	P2_Image->SetOpacity(1.0f);
	P2_Name->SetOpacity(1.0f);
	P2_Name->SetShadowColorAndOpacity(P2_Name->GetShadowColorAndOpacity().CopyWithNewOpacity(1.0f));

	FixScrollToEnd();
}


void UDialogWidget::LoadDialog(FName DialogName)
{
	if (!DialogDataTable) return;

	FDialogData* DialogData = DialogDataTable->FindRow<FDialogData>(DialogName, TEXT(""));

	if (!DialogData) return;

	ClearMessages();

	for (const FMessageData& Message : DialogData->Messages)
	{
		DialogQueue.Enqueue(Message);
	}

	AdvanceDialog();
}

bool UDialogWidget::HasDialog(FName DialogName)
{
	if (!DialogDataTable) return false;

	auto DialogData = DialogDataTable->FindRow<FDialogData>(DialogName, TEXT(""));

	if (!DialogData) return false;

	return true;
}

void UDialogWidget::ClearMessages()
{
	TextContainer->ClearChildren();
	//DialogArray.Empty();
	DialogQueue.Empty();
}

void UDialogWidget::AdvanceDialog()
{
	if (!DialogQueue.IsEmpty())
	{
		FMessageData Message;
		DialogQueue.Dequeue(Message);

		if (!Message.TriggerName.IsEmpty())
		{
			for (auto Trigger : Message.TriggerName)
			{
				if (!Trigger.IsNone())
				{
					UDialogManager::OnDialogTriggered.Broadcast(Trigger);
				}
			}
		}

		if (!Message.DialogText.IsEmpty())
		{
			if (Message.DialogPlayer == EDialogPlayer::Player_1)
			{
				AddTextPlayer1(Message);
			}
			else
			{
				AddTextPlayer2(Message);
			}
		}

		if (auto SoundArray = DialogSFX.Find(Message.SpeakerName.ToString()))
		{
			int32 RandomIndex = FMath::RandRange(0, SoundArray->Sounds.Num() - 1);
			auto Sound = SoundArray->Sounds[RandomIndex];
			UGameplayStatics::PlaySound2D(this, Sound);
		}
	}
	else
	{
		UDialogManager::OnDialogTriggered.Broadcast(FName("EndDialog"));

		RemoveFromParent();
		if (auto GM = Cast<AColorLeakGameMode>(GetWorld()->GetAuthGameMode()))
		{
			GM->SetDialogActive(false);
			GM->SetPlayerMovementEnabled(true);
		}
	}
}
