#include "Objects/Easel/EaselActor.h"
#include "ColorLeakGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Lobby/Character/LobbyCharacter.h"
#include "Tracks/MovieSceneTransformTrack.h"
#include "UI/Easel/EaselUI.h"
#include "Camera/CameraActor.h"
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "MovieScene.h"
#include "MovieSceneSection.h"
#include "Camera/CameraComponent.h"
#include "Sequence/SequenceManager.h"

// Sets default values
AEaselActor::AEaselActor()
{
    EaselMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EaselMeshComponent"));
    RootComponent = EaselMeshComponent;

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    BoxComponent->SetupAttachment(RootComponent);

    EaselWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EaselWidgetComponent"));
    EaselWidgetComponent->SetupAttachment(RootComponent);

    InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
    InteractionWidget->SetupAttachment(RootComponent);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEaselActor::BeginPlay()
{
    Super::BeginPlay();

    this->OnActorBeginOverlap.AddDynamic(this, &AEaselActor::OnOverlap);
    this->OnActorEndOverlap.AddDynamic(this, &AEaselActor::OnEndOverlap);

    if (EaselWidgetComponent)
    {
        if (auto EaselWidget = Cast<UEaselUI>(EaselWidgetComponent->GetWidget()))
        {
            if (!UColorLeakGameInstance::GetLevelName(Level).IsEmpty())
            {
                EaselWidget->SetText(UColorLeakGameInstance::GetLevelName(Level));
            }

            if (ImageBrush.GetResourceObject())
            {
                EaselWidget->SetImageBrush(ImageBrush);
            }
        }
    }

    if (InteractionWidget)
    {
        InteractionWidget->SetVisibility(false);
    }

    if (auto GI = Cast<UColorLeakGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
    {
        if (bIsShowOnStart || GI->IsLevelFinished(UColorLeakGameInstance::GetPreviousLevel(Level)))
        {
            ShowWidget(true);
        }
        else
        {
            ShowWidget(false);
        }
    }


    if (!LobbyCharacter)
    {
        LobbyCharacter = Cast<ALobbyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    }
}

void AEaselActor::OnOverlap(AActor* MyActor, AActor* OtherActor)
{
    if (!MyActor || !OtherActor) return;

    if (auto Player = Cast<ALobbyCharacter>(OtherActor))
    {
        if (IsShowWidget())
        {
            InteractionWidget->SetVisibility(true);
            Player->SetEaselActor(this);
        }
    }
}

void AEaselActor::OnEndOverlap(AActor* MyActor, AActor* OtherActor)
{
    if (!MyActor || !OtherActor) return;

    if (auto Player = Cast<ALobbyCharacter>(OtherActor))
    {
        if (IsShowWidget())
        {
            InteractionWidget->SetVisibility(false);
            Player->SetEaselActor(nullptr);
        }
    }
}

void AEaselActor::ShowWidget(bool bShow)
{
    if (EaselWidgetComponent)
    {
        EaselWidgetComponent->SetVisibility(bShow);
    }
}

bool AEaselActor::IsShowWidget()
{
    return EaselWidgetComponent->IsVisible();
}

void AEaselActor::Interact()
{
    if (!UColorLeakGameInstance::GetDevLevelName(Level).IsNone())
    {
        // TODO finish that
        // Create and play the sequence
        /*ULevelSequence* Sequence = CreateNewLevelSequence();
        AddCameraKeyframes(Sequence);
        PlaySequence(Sequence);

        // Bind the OnFinished delegate to a lambda function that opens the level
        if (SequencePlayer)
        {
            SequencePlayer->OnFinished.AddDynamic(this, &AEaselActor::OnSequenceFinished);
        }*/
        PlaySequence();
        //OnSequenceFinished();
    }
}

void AEaselActor::PlaySequence()
{

    if (auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        PlayerController->SetViewTargetWithBlend(CameraActor, 0.0f, EViewTargetBlendFunction::VTBlend_Cubic);
    }

    UMovieScene* MovieScene;
    LevelSequence = SequenceManager::CreateSequence(MovieScene, FrameRate, SequenceLengthInSeconds);

    FGuid BindingID;
    SequenceManager::BindObjectToSequence(MovieScene, BindingID, CameraActor, LevelSequence);

    UMovieScene3DTransformSection* TransformSection;
    SequenceManager::AddTransformTrack(MovieScene, BindingID, TransformSection);

    auto Cam = LobbyCharacter->GetFollowCamera();

    SequenceManager::AddKeyFrames(MovieScene, TransformSection, Cam->GetComponentTransform(), 0.0);

    SequenceManager::AddKeyFrames(MovieScene, TransformSection, CameraComponent->GetComponentTransform(), SequenceLengthInSeconds);

    FMovieSceneSequencePlaybackSettings PlaybackSettings;
    PlaybackSettings.FinishCompletionStateOverride = EMovieSceneCompletionModeOverride::ForceKeepState;
    ALevelSequenceActor* SequenceActor = nullptr;
    ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), LevelSequence, PlaybackSettings, SequenceActor);

    if (SequencePlayer)
    {
        SequencePlayer->Play();

        SequencePlayer->OnFinished.AddDynamic(this, &AEaselActor::OnSequenceFinished);
    }
}

void AEaselActor::OnSequenceFinished()
{
    if (!UColorLeakGameInstance::GetDevLevelName(Level).IsNone())
    {
        UGameplayStatics::OpenLevel(this, UColorLeakGameInstance::GetDevLevelName(Level), true);
    }
}
