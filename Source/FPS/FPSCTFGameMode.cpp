// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSCTFGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "PlayerManager.h"
#include "FPSCTFPlayerState.h"
#include "VRPlayerController.h"

#include "FPSGameStateBase.h"

AFPSCTFGameMode::AFPSCTFGameMode ()
{
    MaxPlayersPerTeam = 5;
}

void AFPSCTFGameMode::SetTeam (AVRPlayerController* PlayerController, ETeamType Type)
{
    int Index = PlayerControllers.Find (PlayerController);

    if (Index != INDEX_NONE)
    {
        Cast<AFPSCTFPlayerState> (PlayerControllers[Index]->PlayerState)->TeamType = Type;
    }
}

void AFPSCTFGameMode::StartMatch ()
{
    FindCTFSpawns ();
    DestroyDMActors ();

    FString TeamTypeStr = UGameplayStatics::ParseOption (OptionsString, "TeamType");

    if (TeamTypeStr.Equals (FString ("Red")))
    {
        SetTeam (Cast<AVRPlayerController> (UGameplayStatics::GetPlayerController (GetWorld (), 0)), ETeamType::Red);
    }
    else if (TeamTypeStr.Equals (FString ("Blue")))
    {
        SetTeam (Cast<AVRPlayerController> (UGameplayStatics::GetPlayerController (GetWorld (), 0)), ETeamType::Blue);
    }

    SpawnPlayers ();

    Cast<AFPSGameStateBase> (UGameplayStatics::GetGameState (GetWorld ()))->MatchType = EMatchType::CaptureTheFlag;

    Super::StartMatch ();
}

void AFPSCTFGameMode::FindCTFSpawns ()
{
    TArray<AActor*> CTFActors;
    UGameplayStatics::GetAllActorsWithTag (GetWorld (), FName ("CTF"), CTFActors);

    for (int a = 0; a < CTFActors.Num (); a++) 
    {
        if (CTFActors[a]->ActorHasTag (FName ("PlayerManager")))
        {
            if (CTFActors[a]->ActorHasTag (FName ("Red")))
            {
                RedPlayerManager = Cast <APlayerManager> (CTFActors[a]);
            }
            else if (CTFActors[a]->ActorHasTag (FName ("Blue")))
            {
                BluePlayerManager = Cast <APlayerManager> (CTFActors[a]);
            }
        }
    }
}

void AFPSCTFGameMode::DestroyDMActors ()
{
    TArray<AActor*> DMActors;
    UGameplayStatics::GetAllActorsWithTag (GetWorld (), FName ("DM"), DMActors);

    for (int a = 0; a < DMActors.Num (); a++)
    {
        DMActors[a]->Destroy ();
    }
}

void AFPSCTFGameMode::SpawnPlayers ()
{
    for (int pc = 0; pc < PlayerControllers.Num (); pc++)
    {
        SpawnPlayer (PlayerControllers[pc]);
    }
}

void AFPSCTFGameMode::SpawnPlayer (AVRPlayerController* PlayerController)
{
    AFPSCTFPlayerState* FPSCTFPlayerState = Cast <AFPSCTFPlayerState> (PlayerController->PlayerState);

    if (FPSCTFPlayerState->TeamType == ETeamType::Red)
    {
        PlayerController->OnPossess (Cast<APawn> (RedPlayerManager->SpawnPlayer ()));
    }
    else if (FPSCTFPlayerState->TeamType == ETeamType::Blue)
    {
        PlayerController->OnPossess (Cast<APawn> (BluePlayerManager->SpawnPlayer ()));
    }
}

void AFPSCTFGameMode::PlayerKilled (AVRPlayerController* PlayerController)
{
    FTimerHandle RespawnTimerHandle;
    FTimerDelegate RespawnTimerDelegate;

    RespawnTimerDelegate.BindUFunction (this, FName ("OnRespawnTimeOver"), PlayerController);
    GetWorldTimerManager ().SetTimer (RespawnTimerHandle, RespawnTimerDelegate, MinRespawnDelay, false);    
}

void AFPSCTFGameMode::OnRespawnTimeOver (AVRPlayerController* PlayerController)
{
    PlayerController->GetPawn ()->Destroy ();
    SpawnPlayer (PlayerController);
}