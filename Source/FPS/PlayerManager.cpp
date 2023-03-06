// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerManager.h"

#include "Components/BoxComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "NavigationSystem.h"

#include "BOT.h"
#include "VRPlayer.h"
#include "FPSGameModeBase.h"
#include "BOTAIController.h"

APlayerManager::APlayerManager ()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	NumberOfVRPlayers = 0;
	NumberOfBOTs = 0;

	VRPlayerClass = AVRPlayer::StaticClass ();
	BOTClass = ABOT::StaticClass ();

	SpawnBox = CreateDefaultSubobject <UBoxComponent> (FName ("Spawn Box"));
	SpawnBox->SetCollisionProfileName (FName ("NoCollision"));
	SpawnBox->SetupAttachment (GetRootComponent ());
}

void APlayerManager::SpawnPlayers ()
{
	/*for (int v = 0; v < NumberOfVRPlayers; v++) 
	{
		SpawnPlayer (EPlayerType::VRPlayer);
	}

	for (int b = 0; b < NumberOfBOTs; b++) 
	{
		SpawnPlayer (EPlayerType::BOT);
	}*/
}

AActor* APlayerManager::SpawnPlayer ()
{
	const FTransform SpawnTransform (FindSpawnLocation ());

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	return GetWorld ()->SpawnActor (VRPlayerClass, &SpawnTransform, SpawnParams);
}

/*void APlayerManager::SpawnPlayer (EPlayerType Type)
{
	FVector SpawnPoint;
	
	if (!FindSpawnLocation (SpawnPoint))
	{
		return;
	}

	////GEngine->AddOnScreenDebugMessage (DebugLogCounter++, 2.f, FColor::Black, TEXT ("Player Spawning"));
	
	FTransform SpawnTransform (SpawnPoint);

	APlayerBase* PlayerActor = nullptr;
	if (Type == EPlayerType::VRPlayer)
	{
		PlayerActor = Cast<APlayerBase> (UGameplayStatics::BeginDeferredActorSpawnFromClass (GetWorld (), VRPlayerClass, SpawnTransform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn, this));
	}
	else if (Type == EPlayerType::BOT)
	{
		PlayerActor = Cast<APlayerBase> (UGameplayStatics::BeginDeferredActorSpawnFromClass (GetWorld (), BOTClass, SpawnTransform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn, this));
	}
	
	if (PlayerActor)
	{
		PlayerActor->PlayerManager = this;

		UGameplayStatics::FinishSpawningActor (PlayerActor, SpawnTransform);
		PlayersInPlay.Add (PlayerActor);
	}
}*/

void APlayerManager::SpawnBOT ()
{
	const FTransform SpawnTransform (FindSpawnLocation ());

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	GetWorld ()->SpawnActor (BOTClass, &SpawnTransform, SpawnParams);
}

FVector APlayerManager::FindSpawnLocation ()
{
	FVector OutSpawnLocation;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent <UNavigationSystemV1> (GetWorld ());

	FVector RandomPointInBox = UKismetMathLibrary::RandomPointInBoundingBox (SpawnBox->GetComponentLocation (), SpawnBox->GetScaledBoxExtent ());

	FNavLocation SpawnLocation;
	bool IsSpawnPointFound = NavSys->ProjectPointToNavigation (RandomPointInBox, SpawnLocation, FVector (1000, 1000, 1000));
	if (!IsSpawnPointFound) 
	{
		int NumTries = 10;

		for (int t = 0; t < NumTries; t++)
		{
			IsSpawnPointFound = NavSys->ProjectPointToNavigation (RandomPointInBox, SpawnLocation, FVector (1000, 1000, 1000));

			if (IsSpawnPointFound) 
			{
				break;
			}
		}
	}

	OutSpawnLocation = SpawnLocation.Location;
	OutSpawnLocation.Z = 95.f;

	return OutSpawnLocation;
}

bool APlayerManager::FindSpawnLocation (FVector &OutSpawnLocation)
{
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent <UNavigationSystemV1> (GetWorld ());

	FVector RandomPointInBox = UKismetMathLibrary::RandomPointInBoundingBox (SpawnBox->GetComponentLocation (), SpawnBox->GetScaledBoxExtent ());

	FNavLocation SpawnLocation;
	bool IsSpawnPointFound = NavSys->ProjectPointToNavigation (RandomPointInBox, SpawnLocation, FVector (1000, 1000, 1000));
	if (!IsSpawnPointFound) 
	{
		int NumTries = 10;

		for (int t = 0; t < NumTries; t++)
		{
			IsSpawnPointFound = NavSys->ProjectPointToNavigation (RandomPointInBox, SpawnLocation, FVector (1000, 1000, 1000));

			if (IsSpawnPointFound) 
			{
				break;
			}
		}
	}

	OutSpawnLocation = SpawnLocation.Location;
	OutSpawnLocation.Z = 88.f;

	return IsSpawnPointFound;
}

void APlayerManager::RespawnPlayer (APlayerBase* DeadPlayer)
{
/*	if (DeadPlayer->IsA (AVRPlayer::StaticClass ()))
	{
		SpawnPlayer (EPlayerType::VRPlayer);
	}
	else
	{
		SpawnPlayer (EPlayerType::BOT);
	}*/
}

/*void APlayerManager::PlayerKilled (APlayerBase* DeadPlayer)
{
	////GEngine->AddOnScreenDebugMessage (DebugLogCounter++, 2.f, FColor::Yellow, TEXT ("Killed"));
	////GEngine->AddOnScreenDebugMessage (DebugLogCounter++, 2.f, FColor::Yellow, *DeadPlayer->GetName ());

	FTimerHandle RespawnTimerHandle;
	FTimerDelegate RespawnTimerDelegate;
	
	RespawnTimerDelegate.BindUFunction (this, FName ("RespawnPlayer"), DeadPlayer);
	
	AFPSGameModeBase* GameMode = Cast<AFPSGameModeBase> (UGameplayStatics::GetGameMode (GetWorld ()));
	GetWorldTimerManager ().SetTimer (RespawnTimerHandle, RespawnTimerDelegate, GameMode->MinRespawnDelay, false);
}*/