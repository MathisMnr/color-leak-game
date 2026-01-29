#pragma once

#include "CoreMinimal.h"
#include "ColorLeak/ColorLeakCharacter.h"
#include "ColorLeak/ColorLeakGameMode.h"
#include "Chromarion.h"
#include "UI/Widget/AcrylUI.h"
#include "Acryl.generated.h"


/**
 * 
 */
class AChromarion;

UCLASS()
class AAcryl : public AColorLeakCharacter
{
	GENERATED_BODY()

private:
	// Because we already have an Overlap function in the parent class, we don't have to redefine "UFUNCTION()" here
    virtual void OnOverlap(AActor* MyActor, AActor* OtherActor) override;
	
    virtual void OnEndOverlap(AActor* MyActor, AActor* OtherActor) override;

public:
	AAcryl();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Acryl")
	virtual void DeathCharacter() override;

	EColorType GetColorBrush(FName SocketName);
	void SetColorBrush();
	void ClearBrush();
	
	FString colorName;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PaintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ChangePaintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* PaintMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UAcrylUI* AcrylUI;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> UIClass;

	UFUNCTION()
	void OnPaintAnimationEnded(UAnimMontage* Montage, bool bInterrupted);

	void Paint();

	void ChangeColorBrush();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* PaintSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* PaintObjectSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DeathAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
    USoundBase* DeathSound;

    UPROPERTY(EditDefaultsOnly, Category="Respawn")
    TSubclassOf<APawn> RespawnClass;

	UFUNCTION()
	void OnDeathAnimationEnded(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY(EditAnywhere, Category = "Feedback")
	class UForceFeedbackEffect* ForceFeedbackEffect;
};
