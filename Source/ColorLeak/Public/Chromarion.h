#pragma once

#include "CoreMinimal.h"
#include "ColorTypes.h"
#include "ColorLeak/ColorLeakCharacter.h"
#include "ColorLeak/ColorLeakGameMode.h"
#include "Components/Image.h"
#include "Components/WidgetComponent.h"
#include "Components/WidgetSwitcher.h"
#include "Interface/PaintableInterface.h"
#include "UI/Widget/SprintTime.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Components/TimelineComponent.h"
#include "Chromarion.generated.h"

UCLASS()
class AChromarion : public AColorLeakCharacter, public IPaintableInterface
{
	GENERATED_BODY()

	/** Power Up Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PowerUpAction;

	int PowerLimit = 0;

	enum PowerLimitUsage {
		JumpLimitUsage = 3,
		SpeedLimitUsage = 3,
		DashLimitUsage = 5,
		GravityLimitUsage = 6,
		ScaleLimitUsage = 10,
	};

	TArray<int> PowerLimitArray = { 0, JumpLimitUsage, SpeedLimitUsage, DashLimitUsage, GravityLimitUsage, ScaleLimitUsage, 0 };

	bool bCanDash = true;

	bool bCanSpeed = true;

	double SpeedTime = 1.5;

	FVector2D LastMovementInput = FVector2D::ZeroVector;

	// widget class
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> PowerUpWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> SprintTimeWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> ShrinkPositionWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Feedback")
	class UForceFeedbackEffect* ForceFeedbackEffect;

public:
	AChromarion();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Color")
	EColorType GetCurrentColor();

	UFUNCTION(BlueprintCallable, Category = "Color")
	virtual void SetCurrentColor(EColorType newColor) override;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetPowerUpWidgetLimit(int32 NewPowerLimit);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetPowerUpWidget(int32 PowerUp);

	UFUNCTION(BlueprintCallable)
	void SetPowerLimit(int32 NewPowerLimit);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* SprintTimeWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* ShrinkPositionWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* DashTrailComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* SpeedTrailComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* JumpEffectComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* GravityEffectComponent;

	bool bIsPowerActive = false;

	bool bIsOnGround = true;

	UFUNCTION(BlueprintCallable, Category = "Chromarion")
	virtual void DeathCharacter() override;

	void ResetGravity();

	FTimeline ScaleEffectTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* ScaleEffectCurve;

	UFUNCTION()
	void HandleScaling(float Value);

protected:
	virtual void Move(const FInputActionValue& Value) override;

	/** Called for power up input */
	void PowerUp(const FInputActionValue& Value);

	virtual void StartJump() override;

	virtual void Landed(const FHitResult& Hit) override;

	void ResetPowerEffect();

	void ResetColor();

	void PowerUsage();

	void LimitVelocity();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DeathAnimation;

    UPROPERTY(EditDefaultsOnly, Category="Respawn")
    TSubclassOf<APawn> RespawnClass;

	UFUNCTION()
	void OnDeathAnimationEnded(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
	bool bIsDead;

	void UpdateShrinkWidgetPosition();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Color", meta = (AllowPrivateAccess = "true"))
	EColorType CurrentColor;

	UPROPERTY()
	UUserWidget* PowerUpWidget;

	UPROPERTY()
	UWidgetSwitcher* PowerUpSwitcher;

	UPROPERTY()
	USprintTime* SprintTimeWidget;

	UPROPERTY()
	UUserWidget* ShrinkWidget;

// SFX for Chromarion //
	
protected:
	// Sound effects for the jump
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* JumpSoundOne;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* JumpSoundTwo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* LongJumpSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* LandSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* DashSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* DashShoutSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* SpeedPowerUpSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* DeathSound1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* DeathSound2;

};
