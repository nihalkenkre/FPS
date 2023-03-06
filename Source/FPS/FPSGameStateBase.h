// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"

#include "AllEnums.h"

#include "FPSGameStateBase.generated.h"

UCLASS()
class FPS_API AFPSGameStateBase : public AGameState
{
	GENERATED_BODY ()
	
public:
	AFPSGameStateBase ();

	// UPROPERTY (EditAnywhere)
	// 	int RedTeamFlagCaptures;

	// UPROPERTY (EditAnywhere)
	// 	int BlueTeamFlagCaptures;

	// UPROPERTY (EditAnywhere)
	// 	int RedTeamFlagReturns;

	// UPROPERTY (EditAnywhere)
	// 	int BlueTeamFlagReturns;

	// UPROPERTY (EditAnywhere)
	// 	int RedTeamKills;

	// UPROPERTY (EditAnywhere)
	// 	int BlueTeamKills;

	// UPROPERTY (EditAnywhere, BlueprintReadOnly)
	// 	float TimeRemainingInSeconds;

	// UPROPERTY (EditAnywhere, BlueprintReadOnly)
	// 	float TimeElapsedInSeconds;

	// UPROPERTY (EditAnywhere)
	// 	float WarmupTime;

	// UPROPERTY (EditAnywhere)
	// 	float RespawnTime;

	UPROPERTY (EditAnywhere, BlueprintReadOnly)
		EMatchType MatchType;
};
