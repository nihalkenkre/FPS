// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSDMGameMode.h"

#include "GameFramework/PlayerStart.h"

#include "Kismet/GameplayStatics.h"

#include "PlayerManager.h"
#include "VRPlayerController.h"
#include "BOT.h"
#include "FPSGameStateBase.h"

AFPSDMGameMode::AFPSDMGameMode ()
{
    MaxNumberOfPlayers = 10;
    MaxTimeLimitInSeconds = 600;
}

void AFPSDMGameMode::StartMatch ()
{
    FindDMSpawns ();
    DestroyCTFActors ();

    SpawnPlayers ();
    SpawnBOTs ();

    Cast<AFPSGameStateBase> (UGameplayStatics::GetGameState (GetWorld ()))->MatchType = EMatchType::Deathmatch;

    Super::StartMatch ();
}

void AFPSDMGameMode::FindDMSpawns ()
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

void AFPSDMGameMode::DestroyCTFActors ()
{
    TArray<AActor*> CTFActors;
    UGameplayStatics::GetAllActorsWithTag (GetWorld (), FName ("CTF"), CTFActors);

    for (int a = 0; a < CTFActors.Num (); a++)
    {
        CTFActors[a]->Destroy ();
    }
}

void AFPSDMGameMode::SpawnPlayers ()
{
    for (int pc = 0; pc < PlayerControllers.Num (); pc++)
    {
        SpawnPlayer (PlayerControllers[pc]);
    }
}

void AFPSDMGameMode::SpawnBOTs ()
{
    MaxBots = MaxNumberOfPlayers - PlayerControllers.Num ();

    while (NumBots < MaxBots)
    {
        SpawnBOT ();
    }
}

void AFPSDMGameMode::SpawnPlayer (AVRPlayerController* PlayerController)
{
    PlayerController->OnPossess (Cast<APawn> (DMPlayerManager->SpawnPlayer ()));
}

void AFPSDMGameMode::SpawnBOT ()
{
    DMPlayerManager->SpawnBOT ();
    NumBots++;
}

void AFPSDMGameMode::PlayerKilled (AVRPlayerController* PlayerController)
{
    FTimerHandle RespawnTimerHandle;
    FTimerDelegate RespawnTimerDelegate;

    RespawnTimerDelegate.BindUFunction (this, FName ("OnPlayerRespawnTimeOver"), PlayerController);
    GetWorldTimerManager ().SetTimer (RespawnTimerHandle, RespawnTimerDelegate, MinRespawnDelay, false);
}

void AFPSDMGameMode::BOTKilled (ABOT* BOT)
{
    FTimerHandle RespawnTimerHandle;
    FTimerDelegate RespawnTimerDelegate;

    RespawnTimerDelegate.BindUFunction (this, FName ("OnBOTRespawnTimeOver"), BOT);
	GetWorldTimerManager ().SetTimer (RespawnTimerHandle, RespawnTimerDelegate, MinRespawnDelay, false);

    NumBots--;
}

void AFPSDMGameMode::OnPlayerRespawnTimeOver (AVRPlayerController* PlayerController)
{
    PlayerController->GetPawn ()->Destroy ();
    SpawnPlayer (PlayerController);
}

void AFPSDMGameMode::OnBOTRespawnTimeOver (ABOT* BOT)
{
    BOT->Destroy ();
    
    if (NumBots < MaxBots)
    {
        SpawnBOT ();
    }
}