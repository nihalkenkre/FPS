// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "AllEnums.h"

#include "FPSGameInstance.generated.h"

UCLASS()
class FPS_API UFPSGameInstance : public UGameInstance
{
	GENERATED_BODY ()

public:
	UPROPERTY (EditAnywhere, BlueprintReadWrite)
		ETeamType TeamType;
};
