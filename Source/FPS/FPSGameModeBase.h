// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "AllEnums.h"

#include "FPSGameModeBase.generated.h"

class AVRPlayerController;
class APlayerManager;

UCLASS()
class FPS_API AFPSGameModeBase : public AGameMode
{
	GENERATED_BODY ()

public:
	AFPSGameModeBase ();
	
	UPROPERTY (EditAnywhere, BlueprintReadWrite)
		int MaxTimeLimitInSeconds;

	UPROPERTY (EditAnywhere, BlueprintReadWrite)
		int WarmupTime;

	/*UPROPERTY (EditAnywhere, BlueprintReadWrite)
		EMatchType MatchType;

	UPROPERTY (EditAnywhere, BlueprintReadWrite)
		bool IsReadyToStartMatch;

	UFUNCTION (BlueprintCallable)
		void SetTeam (AVRPlayerController* PlayerController, ETeamType Type);

	void PlayerKilled (AVRPlayerController* PlayerController);*/

	virtual void PostLogin (APlayerController* PlayerController) override;

	/*bool ReadyToStartMatch_Implementation () override;
	virtual void StartMatch () override;

	void SpawnPlayers ();
	void SpawnPlayer (AVRPlayerController* PlayerController);*/

	virtual void HandleMatchIsWaitingToStart () override;
	virtual void HandleMatchHasStarted () override;
	virtual void HandleMatchAborted () override;
	virtual void HandleMatchHasEnded () override;
	virtual void HandleLeavingMap () override;

protected:
	void GameTimerElapsed ();

	/*void FindDMSpawn ();
	void FindCTFSpawns ();
	
	void DestroyDMActors ();
	void DestroyCTFActors ();

	UFUNCTION ()
		void OnRespawnTimeOver (AVRPlayerController* PlayerController);*/

	FTimerHandle GameTimerHandle;

	TArray <AVRPlayerController*> PlayerControllers;

	/*APlayerManager* DMPlayerManager;
	APlayerManager* CTFRedPlayerManager;
	APlayerManager* CTFBluePlayerManager;*/
};
