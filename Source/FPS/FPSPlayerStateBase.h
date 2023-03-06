// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "AllEnums.h"

#include "FPSPlayerStateBase.generated.h"

UCLASS()
class FPS_API AFPSPlayerStateBase : public APlayerState
{
	GENERATED_BODY ()
	
public:
	AFPSPlayerStateBase ();

	UPROPERTY (EditAnywhere)
		int NumberOfKills;

	UPROPERTY (EditAnywhere)
		EPlayerPlayState PlayerPlayState;
};
