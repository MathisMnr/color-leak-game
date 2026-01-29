// Copyright Epic Games, Inc. All Rights Reserved.

#include "ColorLeakCharacter.h"

#include "ColorLeakGameMode.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Mechanism/Lever.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AColorLeakCharacter

AColorLeakCharacter::AColorLeakCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	/*GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;*/

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = false; // Don't rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	// Bind function OnActorBeginOverlap with your class function OnOverlap
	this->OnActorBeginOverlap.AddDynamic(this, &AColorLeakCharacter::OnOverlap);
	// Bind function OnActorBeginOverlap with your class function EndOverlap
	this->OnActorEndOverlap.AddDynamic(this, &AColorLeakCharacter::OnEndOverlap);
}

void AColorLeakCharacter::BeginPlay(){

	Super::BeginPlay();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Begin Play !! ")));

	if (PauseMenuClass)
    {
        PauseMenuWidget = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(this, 0), PauseMenuClass);
    }
}

//////////////////////////////////////////////////////////////////////////
// Input

void AColorLeakCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AColorLeakCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AColorLeakCharacter::StartJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AColorLeakCharacter::EndJump);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AColorLeakCharacter::Move);

		// Change menu
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AColorLeakCharacter::ChangeLeverState);

		// Menu Pause
		EnhancedInputComponent->BindAction(MenuAction, ETriggerEvent::Started, this, &AColorLeakCharacter::PauseMenu);

		// Dialog
		EnhancedInputComponent->BindAction(DialogAction, ETriggerEvent::Completed, this, &AColorLeakCharacter::AdvanceDialog);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AColorLeakCharacter::Move(const FInputActionValue& Value)
{
	// Input est un Vector2D, on prend la composante Y pour la direction haut/bas
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(FVector(0.0f, 1.0f, 0.0f), MovementVector.X);
	}
}

void AColorLeakCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AColorLeakCharacter::AdvanceDialog(const FInputActionValue& Value)
{
	// get the GM
	if (auto GM = Cast<AColorLeakGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GM->AdvanceDialog();
	}
}

void AColorLeakCharacter::StartJump()
{
	if (!bCanJump) return;

	Super::Jump();
	isJumping = true;	
}

void AColorLeakCharacter::EndJump()
{
	Super::StopJumping();
	isJumping = false;

}

void AColorLeakCharacter::ChangeLeverState(const FInputActionValue& Value)
{
	
	if (IsOverlapping)
	{		// Get the Overlapping Actor
		if (this->OverlappingActor)
		{		// Cast the Overlapping Actor to ALever
			ALever* Lever = Cast<ALever>(OverlappingActor);
			if (Lever)
			{		// Change the state of the Lever
				Lever->ChangeState(true);
				// log with GEngine Lever->bIsActivated
				FString State = Lever->bIsActivated ? "Activated" : "Deactivated";
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Lever is %s"), *State));
			}
		}
	}
}

void AColorLeakCharacter::PauseMenu(){

	APlayerController* PC = Cast<APlayerController>(GetController());

	if (!PC || !PauseMenuWidget) return;

    if (PauseMenuWidget->IsInViewport())
    {
        PauseMenuWidget->RemoveFromParent();
        PC->SetPause(false); 
        PC->SetInputMode(FInputModeGameOnly()); 
    }
    else 
    {
    	AColorLeakGameMode* GameMode = Cast<AColorLeakGameMode>(GetWorld()->GetAuthGameMode());
    	
    	if (GameMode)
    	{
    		if (!GameMode->dead)
    		{
    			PauseMenuWidget->AddToViewport();
    			PC->SetPause(true); 
    			FInputModeGameAndUI InputMode;
    			InputMode.SetWidgetToFocus(PauseMenuWidget->TakeWidget());
    			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    			PC->SetInputMode(InputMode);

    			PC->SetShowMouseCursor(true);

    			UWidget* FirstWidget = PauseMenuWidget->GetWidgetFromName(TEXT("Restart"));
    			if (FirstWidget)
    			{
    				FirstWidget->SetKeyboardFocus();
    			}
    		}
    	}
    	else
    	{
    		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Erreur GameMode");
    	}

    }
}

//////////////////////////////////////////////////////////////////////////
// Overlap

void AColorLeakCharacter::OnOverlap(AActor* MyActor, AActor* OtherActor){
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Overlap ...")));
	if(OtherActor->IsA(ALever::StaticClass())){
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("... with lever")));
		IsOverlapping = true;
		OverlappingActor = OtherActor;
	}
}

void AColorLeakCharacter::OnEndOverlap(AActor* MyActor, AActor* OtherActor){
	if (OtherActor->IsA(ALever::StaticClass())){
		IsOverlapping = false;
	}
}
