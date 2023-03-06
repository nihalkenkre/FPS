// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSGameModeBase.h"

#include "Kismet/GameplayStatics.h"

#include "FPSGameStateBase.h"
#include "VRPlayerController.h"
#include "AllEnums.h"
#include "PlayerManager.h"
#include "FPSPlayerStateBase.h"

AFPSGameModeBase::AFPSGameModeBase ()
{
    PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;

    MaxTimeLimitInSeconds = 600;

    DefaultPawnClass = nullptr;
    DefaultPlayerName = FText::FromName (FName ("Default Player"));

    MinRespawnDelay = 1;
    WarmupTime = 5;
}

void AFPSGameModeBase::PostLogin (APlayerController* PlayerController)
{
    Super::PostLogin (PlayerController);

    PlayerControllers.Add (Cast<AVRPlayerController> (PlayerController));
}

/*void AFPSGameModeBase::SetTeam (AVRPlayerController* PlayerController, ETeamType Type)
{
    int Index = PlayerControllers.Find (PlayerController);

    if (Index != INDEX_NONE)
    {
        Cast<AFPSPlayerStateBase> (PlayerControllers[Index]->PlayerState)->TeamType = Type;
    }
}


bool AFPSGameModeBase::ReadyToStartMatch_Implementation ()
{
    return IsReadyToStartMatch;
}*/

/*void AFPSGameModeBase::StartMatch ()
{
    if (MatchType == EMatchType::Deathmatch)
    {
        FindDMSpawn ();
        DestroyCTFActors ();

        SpawnPlayers ();
    }
    else if (MatchType == EMatchType::CaptureTheFlag)
    {
        FindCTFSpawns ();
        DestroyDMActors ();

        SpawnPlayers ();
    }

    Super::StartMatch ();
}

void AFPSGameModeBase::FindDMSpawn ()
{
    TArray<AActor*> DMActors;
    UGameplayStatics::GetAllActorsWithTag (GetWorld (), FName ("DM"), DMActors);

    for (int a = 0; a < DMActors.Num (); a++) 
    {
        if (DMActors[a]->ActorHasTag (FName ("PlayerManager")))
        {
            DMPlayerManager = Cast<APlayerManager> (DMActors[a]);
        }
    }
}

void AFPSGameModeBase::FindCTFSpawns ()
{
    TArray<AActor*> CTFActors;
    UGameplayStatics::GetAllActorsWithTag (GetWorld (), FName ("CTF"), CTFActors);

    for (int a = 0; a < CTFActors.Num (); a++) 
    {
        if (CTFActors[a]->ActorHasTag (FName ("PlayerManager")))
        {
            if (CTFActors[a]->ActorHasTag (FName ("Red")))
            {
                CTFRedPlayerManager = Cast <APlayerManager> (CTFActors[a]);
            }
            else if (CTFActors[a]->ActorHasTag (FName ("Blue")))
            {
                CTFBluePlayerManager = Cast <APlayerManager> (CTFActors[a]);
            }
        }
    }
}

void AFPSGameModeBase::DestroyDMActors ()
{
    TArray<AActor*> DMActors;
    UGameplayStatics::GetAllActorsWithTag (GetWorld (), FName ("DM"), DMActors);

    for (int a = 0; a < DMActors.Num (); a++)
    {
        DMActors[a]->Destroy ();
    }
}

void AFPSGameModeBase::DestroyCTFActors ()
{
    TArray<AActor*> CTFActors;
    UGameplayStatics::GetAllActorsWithTag (GetWorld (), FName ("CTF"), CTFActors);

    for (int a = 0; a < CTFActors.Num (); a++)
    {
        CTFActors[a]->Destroy ();
    }
}

void AFPSGameModeBase::SpawnPlayers ()
{
    for (int pc = 0; pc < PlayerControllers.Num (); pc++)
    {
        SpawnPlayer (PlayerControllers[pc]);
    }
}

void AFPSGameModeBase::SpawnPlayer (AVRPlayerController* PlayerController)
{
    if (MatchType == EMatchType::Deathmatch)
    {
        PlayerController->OnPossess (Cast<APawn> (DMPlayerManager->SpawnPlayer ()));
    }
    else if (MatchType == EMatchType::CaptureTheFlag)
    {
        AFPSPlayerStateBase* FPSPlayerState = Cast <AFPSPlayerStateBase> (PlayerController->PlayerState);

        if (FPSPlayerState->TeamType == ETeamType::Red)
        {
            PlayerController->OnPossess (Cast<APawn> (CTFRedPlayerManager->SpawnPlayer ()));
        }
        else if (FPSPlayerState->TeamType == ETeamType::Blue)
        {
            PlayerController->OnPossess (Cast<APawn> (CTFBluePlayerManager->SpawnPlayer ()));
        }
    }
}*/

void AFPSGameModeBase::HandleMatchIsWaitingToStart ()
{
    Super::HandleMatchIsWaitingToStart ();
    
    for (int pc = 0; pc < PlayerControllers.Num (); pc++)
    {
        PlayerControllers[pc]->SetMatchState (EMatchState::WaitingToStart);
    }
}

void AFPSGameModeBase::HandleMatchHasStarted ()
{
    Super::HandleMatchHasStarted ();
    GetWorldTimerManager ().SetTimer (GameTimerHandle, this, &AFPSGameModeBase::GameTimerElapsed, MaxTimeLimitInSeconds, false);

    for (int pc = 0; pc < PlayerControllers.Num (); pc++)
    {
        PlayerControllers[pc]->SetMatchState (EMatchState::Started);
    }    
}

void AFPSGameModeBase::HandleMatchAborted ()
{
    Super::HandleMatchAborted ();
    
    for (int pc = 0; pc < PlayerControllers.Num (); pc++)
    {
        PlayerControllers[pc]->SetMatchState (EMatchState::Aborted);
    }
}

void AFPSGameModeBase::HandleMatchHasEnded ()
{
    Super::HandleMatchHasEnded ();
    
    for (int pc = 0; pc < PlayerControllers.Num (); pc++)
    {
        PlayerControllers[pc]->SetMatchState (EMatchState::Ended);
    }
}

void AFPSGameModeBase::HandleLeavingMap ()
{
    Super::HandleLeavingMap ();
    
    for (int pc = 0; pc < PlayerControllers.Num (); pc++)
    {
        PlayerControllers[pc]->SetMatchState (EMatchState::LeavingMap);
    }
}

void AFPSGameModeBase::GameTimerElapsed ()
{
    EndMatch ();
}

/*void AFPSGameModeBase::PlayerKilled (AVRPlayerController* PlayerController)
{
    FTimerHandle RespawnTimerHandle;
    FTimerDelegate RespawnTimerDelegate;

    RespawnTimerDelegate.BindUFunction (this, FName ("OnRespawnTimeOver"), PlayerController);
    GetWorldTimerManager ().SetTimer (RespawnTimerHandle, RespawnTimerDelegate, MinRespawnDelay, false);    
}

void AFPSGameModeBase::OnRespawnTimeOver (AVRPlayerController* PlayerController)
{
    PlayerController->GetPawn ()->Destroy ();
    SpawnPlayer (PlayerController);
}*/