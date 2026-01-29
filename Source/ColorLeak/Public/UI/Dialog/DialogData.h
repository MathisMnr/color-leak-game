#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DialogData.generated.h"

UENUM(BlueprintType)
enum class EDialogPlayer : uint8
{
	Player_1,
	Player_2
};

USTRUCT(BlueprintType)
struct FMessageData
{
	GENERATED_BODY()

	FMessageData()
	 : MessageID(0), DialogPlayer(EDialogPlayer::Player_1), SpeakerImage(nullptr) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MessageID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDialogPlayer DialogPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DialogText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SpeakerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* SpeakerImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> TriggerName;
};

USTRUCT(BlueprintType)
struct FDialogData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMessageData> Messages;
};