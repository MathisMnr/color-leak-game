// Fill out your copyright notice in the Description page of Project Settings.


#include "Acryl.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Objects/Paint.h"
#include "Objects/MixPaint.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"
#include "Mechanism/Cannon.h"



AAcryl::AAcryl()
{

}

void AAcryl::BeginPlay()
{
	Super::BeginPlay();

	RespawnLocation = GetActorLocation();

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (UIClass)
		{
			UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(PC, UIClass);
			AcrylUI = Cast<UAcrylUI>(WidgetInstance);

			if (AcrylUI)
			{
				AcrylUI->AddToViewport();
			}
		}
	}
}


void AAcryl::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
        if (PaintAction)
		{
			EnhancedInputComponent->BindAction(PaintAction, ETriggerEvent::Triggered, this, &AAcryl::Paint);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("PaintAction NULL !"));
		}
		if (ChangePaintAction)
		{
			EnhancedInputComponent->BindAction(ChangePaintAction, ETriggerEvent::Started, this, &AAcryl::ChangeColorBrush);
		}
		
    }

	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component!"), *GetNameSafe(this));
	}
}

void AAcryl::Paint()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && PaintMontage)
	{
		AnimInstance->Montage_Play(PaintMontage);

		// Define a callback for the animation's end
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AAcryl::OnPaintAnimationEnded);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, PaintMontage);

		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			if (ForceFeedbackEffect)
			{
				PC->ClientPlayForceFeedback(ForceFeedbackEffect);
			}
		}
	}
	
}

EColorType AAcryl::GetColorBrush(FName SocketName)
{
    // Get the Skeletal Mesh
    USkeletalMeshComponent* MeshComp = GetMesh();
    if (MeshComp)
    {
        // Get the component attach at the socket
        UPrimitiveComponent* AttachedComponent = Cast<UPrimitiveComponent>(MeshComp->GetChildComponent(0)); 

        // Check if it exist
        if (AttachedComponent)
        {
            // Get the material
            UMaterialInterface* Material = AttachedComponent->GetMaterial(3);
            if (Material)
            {
                FString MaterialName = Material->GetName();

                if (MaterialName == "M_paintRed"){
                    return EColorType::Red;
                } 
                else if(MaterialName == "M_paintYellow"){
                    return EColorType::Yellow;
                } 
                else if(MaterialName == "M_paintBlue"){
                    return EColorType::Blue;
                }
                else if(MaterialName == "M_paintGreen"){
                    return EColorType::Green;
                } 
                else if(MaterialName == "M_paintOrange"){
                    return EColorType::Orange;
                }
                else if(MaterialName == "M_paintPurple"){
                    return EColorType::Purple;
                } 
                else{
                    return EColorType::Uncolored;
                }
                //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, MaterialName);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("No material at the socket : %s"), *SocketName.ToString());
            }
        }
    }
    return EColorType::Uncolored;
}

void AAcryl::OnPaintAnimationEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == PaintMontage)
	{
		FVector Start = GetActorLocation(); // Start of the raycast
		FVector ForwardVector = GetActorForwardVector(); // Get the direction of the character
		FVector End = Start + (ForwardVector * 800.0f); // Trace on 1000 units

		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this); // Ignore the character

		// Execute the raycast
		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult, 
			Start, 
			End, 
			//ECC_Pawn, // Collision's canal
			ECC_GameTraceChannel1, //First personalized collision canal
			CollisionParams
		);

		//DrawDebugLine(GetWorld(), Start, End, FColor::Green, true, 2.0f, 0, 2.0f);

		// Check if something is hit
		if (bHit)
		{
			FString HitActorName = HitResult.GetActor() ? HitResult.GetActor()->GetName() : TEXT("None");

   			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Actor is : %s"), *HitActorName));

			// Check if the actor hit is paintable
			if (HitResult.GetActor()->ActorHasTag("Paintable")){
				
				EColorType newColor = GetColorBrush("Socket_Brush");

				if (newColor != EColorType::Uncolored){
					IPaintableInterface* PaintableActor = Cast<IPaintableInterface>(HitResult.GetActor()); // Cast the actor to IPaintableInterface or a child of it
					if(PaintableActor){
						
						if(PaintObjectSound){
							UGameplayStatics::SpawnSoundAttached(PaintObjectSound, GetMesh());
						}
						
						PaintableActor->SetCurrentColor(newColor);
					} 
				}
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("Paint animation is done !"));
	}
}



void AAcryl::ChangeColorBrush(){

	if(IsOverlapping){

		// Log with Engine the Overlapping actor's name
		FString OverlappingActorName = OverlappingActor ? OverlappingActor->GetName() : TEXT("None");
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Overlapping actor is : %s"), *OverlappingActorName));
		
		// if the Overlapping actor is a paint
		if (auto Paint = Cast<APaint>(OverlappingActor)){
			// Play the sound
			if(PaintSound){
				UGameplayStatics::SpawnSoundAttached(PaintSound, GetMesh());
			}
		}

		SetColorBrush();
	
		// Mise � jour de l'UI
		if (AcrylUI)
		{
			AcrylUI->UpdateAcrylUI(colorName);
		}

		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			if (ForceFeedbackEffect)
			{
				PC->ClientPlayForceFeedback(ForceFeedbackEffect);
			}
		}
	}
}

void AAcryl::OnOverlap(AActor* MyActor, AActor* OtherActor){
	Super::OnOverlap(MyActor, OtherActor);

	if(auto MixPaint = Cast<AMixPaint>(OtherActor)){

		if(!MixPaint->endMix){

			EColorType colorBrush = GetColorBrush("Socket_Brush");

			if(MixPaint->firstColor == EColorType::Uncolored){
				MixPaint->firstColor = colorBrush;
			} else{
				if(MixPaint->secondColor == EColorType::Uncolored && MixPaint->firstColor != colorBrush){
					MixPaint->secondColor = colorBrush;
				}
			}

			MixPaint->mixColor();

		}

		if (MixPaint->endMix){
			IsOverlapping = true;
			colorName = MixPaint->GetMainColor();
			OverlappingActor = MixPaint;
		}

	}else if(auto Paint = Cast<APaint>(OtherActor)){

		IsOverlapping = true;
		colorName = Paint->GetMainColor();
		OverlappingActor = Paint;
	}
}

void AAcryl::OnEndOverlap(AActor* MyActor, AActor* OtherActor){
	Super::OnEndOverlap(MyActor, OtherActor);

	IsOverlapping = false;
}

void AAcryl::SetColorBrush(){
	
	// Get the Skeletal Mesh
    USkeletalMeshComponent* MeshComp = GetMesh();
    if (MeshComp)
    {
        // Get the component attach at the socket
        UPrimitiveComponent* AttachedComponent = Cast<UPrimitiveComponent>(MeshComp->GetChildComponent(0)); 

        // Check if it exist
        if (AttachedComponent)
        {
			// Gengine to display the colorName
        	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("ColorName is : %s"), *colorName));
        	
			FString MaterialPath = FString::Printf(TEXT("Material'/Game/Characters/Acryl/Brush/M_paint%s.M_paint%s'"), *colorName, *colorName);
			UMaterialInterface* Main = LoadObject<UMaterialInterface>(nullptr, *MaterialPath);

            if (Main){
				AttachedComponent->SetMaterial(3, Main);
			}

		} 
	} 
}

void AAcryl::DeathCharacter(){

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathAnimation)
	{
		GetCharacterMovement()->DisableMovement();

		AnimInstance->Montage_Play(DeathAnimation);

		if(DeathSound){
			UGameplayStatics::SpawnSoundAttached(DeathSound, GetMesh());
		}

		// Define a callback for the animation's end
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AAcryl::OnDeathAnimationEnded);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, DeathAnimation);
	}


	if (auto GM = Cast<AColorLeakGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GM->nbDeathAcryl++;
	}
}

void AAcryl::OnDeathAnimationEnded(UAnimMontage* Montage, bool bInterrupted){

	if(Montage == DeathAnimation){

		APlayerController* PC = Cast<APlayerController>(GetController());

		if(PC){
			AColorLeakGameMode* GM = Cast<AColorLeakGameMode>(GetWorld()->GetAuthGameMode());

			if(GM && RespawnClass){
				FName SpawnTag = "Acryl";
				GM->RespawnPlayer(PC, RespawnClass, SpawnTag);
				GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			}
		}
	}

}

void AAcryl::ClearBrush()
{
	colorName = "Uncolored";
	SetColorBrush(); 
	// Mise � jour de l'UI
	if (AcrylUI)
	{
		AcrylUI->UpdateAcrylUI(colorName);
	}
}




