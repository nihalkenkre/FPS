// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPSGameModeBase.h"
#include "FPSDMGameMode.generated.h"

class APlayerManager;
class ABOT;

UCLASS()
class FPS_API AFPSDMGameMode : public AFPSGameModeBase
{
	GENERATED_BODY ()

public:
	AFPSDMGameMode ();

	UPROPERTY (EditAnywhere, BlueprintReadWrite)
		int MaxNumberOfPlayers;

	virtual void StartMatch () override;

	void SpawnPlayers ();
	void SpawnBOTs ();
	
	void SpawnPlayer (AVRPlayerController* PlayerController);
	void SpawnBOT ();
	
	void PlayerKilled (AVRPlayerController* PlayerController);
	void BOTKilled (ABOT* BOT);

protected:
	void FindDMSpawns ();
	void DestroyCTFActors ();

	UFUNCTION ()
		void OnPlayerRespawnTimeOver (AVRPlayerController* PlayerController);

	UFUNCTION ()
		void OnBOTRespawnTimeOver (ABOT* BOT);

private:
	int MaxBots;
	APlayerManager* DMPlayerManager;
};