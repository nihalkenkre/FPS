// Fill out your copyright notice in the Description page of Project Settings.

#include "VRPlayerController.h"

#include "Components/WidgetComponent.h"

#include "Kismet/GameplayStatics.h"

#include "VRPlayer.h"
#include "FPSDMGameMode.h"
#include "FPSCTFGameMode.h"
#include "FPSDMPlayerState.h"
#include "FPSCTFPlayerState.h"

AVRPlayerController::AVRPlayerController ()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

    TimerWidget = CreateDefaultSubobject <UWidgetComponent> (FName ("Timer Widget"));
    TimerWidget->SetupAttachment (GetRootComponent ());

    AutoReceiveInput = EAutoReceiveInput::Player0;

    SpawnLocation = FVector (100, 100, 100);
}

void AVRPlayerController::BeginPlay ()
{
    Super::BeginPlay ();

    FPSPlayerState = Cast<AFPSPlayerStateBase> (PlayerState);
}

void AVRPlayerController::SetupInputComponent ()
{
    Super::SetupInputComponent ();

    InputComponent->BindAction ("Left Grip", IE_Pressed, this, &AVRPlayerController::LeftGripPressed);
    InputComponent->BindAction ("Left Grip", IE_Released, this, &AVRPlayerController::LeftGripReleased);
    
    InputComponent->BindAction ("Right Grip", IE_Pressed, this, &AVRPlayerController::RightGripPressed);
    InputComponent->BindAction ("Right Grip", IE_Released, this, &AVRPlayerController::RightGripReleased);
    
    InputComponent->BindAction ("Left Trigger", IE_Pressed, this, &AVRPlayerController::LeftTriggerPressed);
    InputComponent->BindAction ("Left Trigger", IE_Released, this, &AVRPlayerController::LeftTriggerReleased);
    
    InputComponent->BindAction ("Right Trigger", IE_Pressed, this, &AVRPlayerController::RightTriggerPressed);
    InputComponent->BindAction ("Right Trigger", IE_Released, this, &AVRPlayerController::RightTriggerReleased);
    
    InputComponent->BindAction ("Left X", IE_Pressed, this, &AVRPlayerController::LeftXPressed);
    InputComponent->BindAction ("Right A", IE_Pressed, this, &AVRPlayerController::RightAPressed);
    
    InputComponent->BindAction ("Thumbstick", IE_Pressed, this, &AVRPlayerController::ThumbstickPressed);
    
    InputComponent->BindAction ("Left Thumbstick Full", IE_Pressed, this, &AVRPlayerController::LeftThumbstickFull);
    InputComponent->BindAction ("Left Thumbstick Full", IE_Released, this, &AVRPlayerController::LeftThumbstickNotFull);

    InputComponent->BindAction ("Menu", IE_Pressed, this, &AVRPlayerController::MenuPressed);
    
    InputComponent->BindAction ("Right Thumbstick Full", IE_Pressed, this, &AVRPlayerController::RightThumbstickFull);
    InputComponent->BindAction ("Right Thumbstick Full", IE_Released, this, &AVRPlayerController::RightThumbstickNotFull);
    
    InputComponent->BindAxis ("Left Thumbstick Up Down", this, &AVRPlayerController::LeftThumbstickUpDown);
    InputComponent->BindAxis ("Left Thumbstick Left Right", this, &AVRPlayerController::LeftThumbstickLeftRight);
    
    InputComponent->BindAxis ("Right Thumbstick Up Down", this, &AVRPlayerController::RightThumbstickUpDown);
    InputComponent->BindAxis ("Right Thumbstick Left Right", this, &AVRPlayerController::RightThumbstickLeftRight);
}

void AVRPlayerController::OnPossess (APawn *InPawn)
{
    Super::OnPossess (InPawn);

    Player = Cast <AVRPlayer> (InPawn);
    
    if (MatchState == EMatchState::Started)
    {
        GetWorldTimerManager ().SetTimer (WarmupTimerHandle, this, &AVRPlayerController::WarmupTimer, 1.f, true, 0.f);
        FPSPlayerState->PlayerPlayState = EPlayerPlayState::Warmup;
        ////GEngine->AddOnScreenDebugMessage (DebugLogCounter++, 2.f, FColor::Blue, TEXT ("Warmup"));
    }
    else if (MatchState == EMatchState::Ended || MatchState == EMatchState::Aborted || MatchState == EMatchState::LeavingMap)
    {
        FPSPlayerState->PlayerPlayState = EPlayerPlayState::None;
    }
}

void AVRPlayerController::SetTeamType (ETeamType Type)
{
    Cast<AFPSCTFPlayerState> (FPSPlayerState)->TeamType = Type;
}

void AVRPlayerController::WarmupTimer ()
{
    WarmupTimeElapsed += GetWorldTimerManager ().GetTimerElapsed (WarmupTimerHandle);

    AFPSGameModeBase* GameMode = Cast<AFPSGameModeBase> (UGameplayStatics::GetGameMode (GetWorld ()));

    if (WarmupTimeElapsed >= GameMode->WarmupTime)
    {
        FPSPlayerState->PlayerPlayState = EPlayerPlayState::Playing;
        ////GEngine->AddOnScreenDebugMessage (DebugLogCounter++, 2.f, FColor::Blue, TEXT ("Playing"));
        GetWorldTimerManager ().ClearTimer (WarmupTimerHandle);
        WarmupTimeElapsed = 0;
    }
    else
    {
    }
}

void AVRPlayerController::OnUnPossess ()
{
    Super::OnUnPossess ();

    Player = nullptr;
}

void AVRPlayerController::SetMatchState (EMatchState State)
{
    MatchState = State;

    if (MatchState == EMatchState::Started)
    {
        //GEngine->AddOnScreenDebugMessage (DebugLogCounter++, 2.f, FColor::Yellow, TEXT ("Match Started"));
        GetWorldTimerManager ().SetTimer (WarmupTimerHandle, this, &AVRPlayerController::WarmupTimer, 1.f, true, 0.f);
        FPSPlayerState->PlayerPlayState = EPlayerPlayState::Warmup;
    }
    else if (MatchState == EMatchState::Ended || MatchState == EMatchState::Aborted || MatchState == EMatchState::LeavingMap)
    {
    }
}

void AVRPlayerController::SetPlayerPlayState (EPlayerPlayState State)
{
    FPSPlayerState->PlayerPlayState = State;

    if (FPSPlayerState->PlayerPlayState == EPlayerPlayState::Dead)
    {
        AFPSGameModeBase* GameMode = Cast<AFPSGameModeBase> (UGameplayStatics::GetGameMode (GetWorld ()));
        
        if (GameMode->IsA (AFPSDMGameMode::StaticClass ()))
        {
            Cast<AFPSDMGameMode> (GameMode)->PlayerKilled (this);
        }
        else if (GameMode->IsA (AFPSCTFGameMode::StaticClass ()))
        {
            Cast<AFPSCTFGameMode> (GameMode)->PlayerKilled (this);
        }
    }
}

void AVRPlayerController::LeftThumbstickUpDown (float AxisValue)
{
    if (Player && FPSPlayerState)
    {
        if (FPSPlayerState->PlayerPlayState == EPlayerPlayState::Playing)
        {
            Player->LeftThumbstickUpDown (AxisValue);
        }
    }
}

void AVRPlayerController::LeftThumbstickLeftRight (float AxisValue)
{
    if (Player && FPSPlayerState)
    {
        if (FPSPlayerState->PlayerPlayState == EPlayerPlayState::Playing)
        {
            Player->LeftThumbstickLeftRight (AxisValue);
        }
    }
}

void AVRPlayerController::RightThumbstickUpDown (float AxisValue)
{
    if (Player && FPSPlayerState)
    {
        if (FPSPlayerState->PlayerPlayState == EPlayerPlayState::Playing)
        {
            Player->RightThumbstickUpDown (AxisValue);
        }
    }
}

void AVRPlayerController::RightThumbstickLeftRight (float AxisValue)
{
    if (Player && FPSPlayerState)
    {
        if (FPSPlayerState->PlayerPlayState == EPlayerPlayState::Playing)
        {
            Player->RightThumbstickLeftRight (AxisValue);
        }
    }
}

void AVRPlayerController::ThumbstickPressed ()
{
    if (Player && FPSPlayerState)
    {
        if (FPSPlayerState->PlayerPlayState == EPlayerPlayState::Playing)
        {
            Player->ThumbstickPressed ();
        }
    }
}

void AVRPlayerController::LeftXPressed ()
{
    if (Player && FPSPlayerState)
    {
        if (FPSPlayerState->PlayerPlayState == EPlayerPlayState::Playing)
        {
            Player->LeftXPressed ();
        }
    }
}

void AVRPlayerController::RightAPressed ()
{
    if (Player && FPSPlayerState)
    {
        if (FPSPlayerState->PlayerPlayState == EPlayerPlayState::Playing)
        {
            Player->RightAPressed ();
        }
    }
}

void AVRPlayerController::LeftGripPressed ()
{
    if (Player && FPSPlayerState)
    {
        if (FPSPlayerState->PlayerPlayState == EPlayerPlayState::Playing)
        {
            Player->LeftGripPressed ();
        }
    }
}

void AVRPlayerController::LeftGripReleased ()
{
    if (Player && FPSPlayerState)
    {
        if (FPSPlayerState->PlayerPlayState == EPlayerPlayState::Playing)
        {
            Player->LeftGripReleased ();
        }
    }
}

void AVRPlayerController::RightGripPressed ()
{
    if (Player && FPSPlayerState)
    {
        if (FPSPlayerState->PlayerPlayState == EPlayerPlayState::Playing)
        {
            Player->RightGripPressed ();
        }
    }
}

void AVRPlayerController::RightGripReleased ()
{
    if (Player && FPSPlayerState)
    {
        if (FPSPlayerState->PlayerPlayState == EPlayerPlayState::Playing)
        {
            Player->RightGripReleased ();
        }
    }
}

void AVRPlayerController::LeftTriggerPressed ()
{
    if (Player && FPSPlayerState)
    {
        Player->LeftTriggerPressed ();
    }
}

void AVRPlayerController::LeftTriggerReleased ()
{
    if (Player && FPSPlayerState)
    {
        Player->LeftTriggerReleased ();
    }
}

void AVRPlayerController::RightTriggerPressed ()
{
    if (Player && FPSPlayerState)
    {
        Player->RightTriggerPressed ();
    }
}

void AVRPlayerController::RightTriggerReleased ()
{
    if (Player && FPSPlayerState)
    {
        Player->RightTriggerReleased ();
    }
}

void AVRPlayerController::MenuPressed ()
{
    if (Player && FPSPlayerState)
    {
        Player->ToggleMenu ();
    }
}

void AVRPlayerController::LeftThumbstickFull ()
{
    if (Player && FPSPlayerState)
    {
        if (FPSPlayerState->PlayerPlayState == EPlayerPlayState::Playing)
        {
            Player->LeftThumbstickFull ();
        }
    }
}

void AVRPlayerController::LeftThumbstickNotFull ()
{
    if (Player && FPSPlayerState)
    {
        if (FPSPlayerState->PlayerPlayState == EPlayerPlayState::Playing)
        {
            Player->LeftThumbstickNotFull ();
        }
    }
}

void AVRPlayerController::RightThumbstickFull ()
{
    if (Player && FPSPlayerState)
    {
        if (FPSPlayerState->PlayerPlayState == EPlayerPlayState::Playing)
        {
            Player->RightThumbstickFull ();
        }
    }
}

void AVRPlayerController::RightThumbstickNotFull ()
{
    if (Player && FPSPlayerState)
    {
        if (FPSPlayerState->PlayerPlayState == EPlayerPlayState::Playing)
        {
            Player->RightThumbstickNotFull ();
        }
    }
}