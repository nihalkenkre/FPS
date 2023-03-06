// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPSGameModeBase.h"
#include "FPSCTFGameMode.generated.h"

class APlayerManager;

UCLASS()
class FPS_API AFPSCTFGameMode : public AFPSGameModeBase
{
	GENERATED_BODY ()

public:
	AFPSCTFGameMode ();

	UPROPERTY (EditAnywhere, BlueprintReadWrite)
		int MaxPlayersPerTeam;

	UFUNCTION (BlueprintCallable)
		void SetTeam (AVRPlayerController* PlayerController, ETeamType Type);

	virtual void StartMatch () override;

	void SpawnPlayers ();
	void SpawnPlayer (AVRPlayerController* PlayerController);
	void PlayerKilled (AVRPlayerController* PlayerController);

protected:
	void FindCTFSpawns ();
	void DestroyDMActors ();

	UFUNCTION ()
		void OnRespawnTimeOver (AVRPlayerController* PlayerController);

private:
	APlayerManager* RedPlayerManager;
	APlayerManager* BluePlayerManager;
};
