// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "FPSMainLevelScriptActor.generated.h"

class APlayerManager;

UCLASS()
class FPS_API AFPSMainLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY ()

public:
	AFPSMainLevelScriptActor ();

	UPROPERTY (EditAnywhere, BlueprintReadWrite)
		APlayerManager* DMPlayerManager;

	UPROPERTY (EditAnywhere, BlueprintReadWrite)
		APlayerManager* CTFRedPlayerManager;

	UPROPERTY (EditAnywhere, BlueprintReadWrite)
		AActor* CTFRedFlag;

	UPROPERTY (EditAnywhere, BlueprintReadWrite)
		APlayerManager* CTFBluePlayerManager;
		
	UPROPERTY (EditAnywhere, BlueprintReadWrite)
		AActor* CTFBlueFlag;

	virtual void BeginPlay () override;

private:
	void DestroyDMActors ();
	void DestroyCTFActors ();
};
