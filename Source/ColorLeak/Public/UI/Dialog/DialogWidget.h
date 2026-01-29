// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogData.h"
#include "Blueprint/UserWidget.h"
#include "DialogWidget.generated.h"

USTRUCT(BlueprintType)
struct FSoundArray
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<USoundBase*> Sounds;
};

/**
 * 
 */
UCLASS()
class COLORLEAK_API UDialogWidget : public UUserWidget
{
	GENERATED_BODY()

	// image player 1
	UPROPERTY(meta = (BindWidget))
	class UImage* P1_Image;

	// image player 2
	UPROPERTY(meta = (BindWidget))
	class UImage* P2_Image;

	// name player 1
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* P1_Name;

	// name player 2
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* P2_Name;

	// text container
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* TextContainer;

	// text container
	UPROPERTY(meta = (BindWidget))
	class UBorder* DialogBorder;

	// data table where row are DialogData
	UPROPERTY(EditAnywhere, Category = "Dialog")
	class UDataTable* DialogDataTable;

	UPROPERTY(EditAnywhere, Category = "UI")
	UDataTable* RichTextStylesheet;

	TQueue<FMessageData> DialogQueue;

	UPROPERTY(EditAnywhere, Category = "UI")
	TMap<FString, FSoundArray> DialogSFX;

	void AddTextPlayer1(const FMessageData& Message);

	void AddTextPlayer2(const FMessageData& Message);

	FTimerHandle ScrollFixTimerHandle;
	int32 ScrollFixCounter = 0;
	void FixScrollToEnd();

public:

	bool HasDialog(FName DialogName);

	void LoadDialog(FName DialogName);

	void AdvanceDialog();

	void ClearMessages();
};
