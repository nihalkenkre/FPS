// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSMainLevelScriptActor.h"

#include "Kismet/GameplayStatics.h"

#include "AllEnums.h"
#include "FPSDMGameMode.h"
#include "VRPlayerController.h"
#include "PlayerManager.h"

#include "FPSCTFGameMode.h"
#include "FPSDMGameMode.h"
#include "FPSGameInstance.h"

AFPSMainLevelScriptActor::AFPSMainLevelScriptActor ()
{
    PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;
}

void AFPSMainLevelScriptActor::BeginPlay ()
{
    Super::BeginPlay ();

    /*AGameModeBase* GameMode = UGameplayStatics::GetGameMode (GetWorld ());

    if (GameMode->IsA (AFPSDMGameMode::StaticClass ())) 
    {
        DestroyCTFActors ();

        AFPSDMGameMode* DMGameMode = Cast <AFPSDMGameMode> (GameMode);

        DMPlayerManager->NumberOfBOTs = DMGameMode->MaxNumberOfPlayers - 1;
        DMPlayerManager->SpawnPlayers ();
    }
    else if (GameMode->IsA (AFPSCTFGameMode::StaticClass ())) 
    {
        DestroyDMActors ();

        AFPSCTFGameMode* CTFGameMode = Cast <AFPSCTFGameMode> (GameMode);
        UFPSGameInstance* GameInstance = Cast <UFPSGameInstance> (UGameplayStatics::GetGameInstance (GetWorld ()));

        if (GameInstance->TeamType == ETeamType::Red)
        {
            CTFRedPlayerManager->NumberOfBOTs = CTFGameMode->MaxPlayersPerTeam - 1;
            CTFBluePlayerManager->NumberOfBOTs = CTFGameMode->MaxPlayersPerTeam;

            CTFRedPlayerManager->SpawnPlayers ();
            CTFBluePlayerManager->SpawnPlayers ();
        }
        else if (GameInstance->TeamType == ETeamType::Blue) 
        {
            CTFBluePlayerManager->NumberOfBOTs = CTFGameMode->MaxPlayersPerTeam - 1;
            CTFRedPlayerManager->NumberOfBOTs = CTFGameMode->MaxPlayersPerTeam;

            CTFRedPlayerManager->SpawnPlayers ();
            CTFBluePlayerManager->SpawnPlayers ();
        }
    }*/
}

void AFPSMainLevelScriptActor::DestroyDMActors ()
{
    DMPlayerManager->Destroy ();
}

void AFPSMainLevelScriptActor::DestroyCTFActors ()
{
    CTFRedPlayerManager->Destroy ();
    CTFRedFlag->Destroy ();

    CTFBluePlayerManager->Destroy ();
    CTFBlueFlag->Destroy ();
}