// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "AllEnums.h"

#include "PlayerManager.generated.h"

class AVRPlayer;
class ABOT;
class APlayerBase;
class UBoxComponent;

UCLASS()
class FPS_API APlayerManager : public AActor
{
	GENERATED_BODY ()
	
public:	
	APlayerManager ();

	UPROPERTY (EditAnywhere, BlueprintReadWrite)
		int NumberOfBOTs;
		
	UPROPERTY (EditAnywhere, BlueprintReadWrite)
		int NumberOfVRPlayers;

	UPROPERTY (VisibleAnywhere)
		UBoxComponent* SpawnBox;

	UPROPERTY (EditAnywhere)
		ETeamType TeamType;

	UPROPERTY (EditAnywhere)
		TSubclassOf <AVRPlayer> VRPlayerClass;

	UPROPERTY (EditAnywhere)
		TSubclassOf <ABOT> BOTClass;
	
	void SpawnPlayers ();
	AActor* SpawnPlayer ();
	//void SpawnPlayer (EPlayerType Type);

	//void PlayerKilled (APlayerBase* DeadPlayer);

	void SpawnBOT ();

	bool FindSpawnLocation (FVector& OutSpawnLocation);
	FVector FindSpawnLocation ();

protected:
	UFUNCTION ()
		void RespawnPlayer (APlayerBase* DeadPlayer);

private:
	TArray<APlayerBase*> PlayersInPlay;
};
