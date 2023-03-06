// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPSPlayerStateBase.h"
#include "FPSCTFPlayerState.generated.h"

UCLASS()
class FPS_API AFPSCTFPlayerState : public AFPSPlayerStateBase
{
	GENERATED_BODY ()

public:	
	UPROPERTY (EditAnywhere)
		int NumberOfFlagsCaptured;
	
	UPROPERTY (EditAnywhere)
		int NumberOfFlagsReturned;

	UPROPERTY (EditAnywhere, BlueprintReadOnly)
		ETeamType TeamType;
};
