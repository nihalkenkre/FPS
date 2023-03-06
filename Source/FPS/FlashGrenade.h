// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrenadeBase.h"
#include "FlashGrenade.generated.h"

UCLASS()
class FPS_API AFlashGrenade : public AGrenadeBase
{
	GENERATED_BODY ()

	virtual void PickUp (ACharacter* PlayerCharacter) override;
	virtual void Drop (ACharacter* PlayerCharacter) override;

protected:
	void OnLifetimeOver ();	
};
