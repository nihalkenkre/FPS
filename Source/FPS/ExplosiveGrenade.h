// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrenadeBase.h"
#include "ExplosiveGrenade.generated.h"

class URadialForceComponent;

UCLASS()
class FPS_API AExplosiveGrenade : public AGrenadeBase
{
	GENERATED_BODY ()

public:
	AExplosiveGrenade ();

	UPROPERTY (VisibleAnywhere)
		URadialForceComponent* RadialForce;

	virtual void PickUp (ACharacter* PlayerCharacter) override;
	virtual void Drop (ACharacter* PlayerCharacter) override;

protected:
	void OnLifetimeOver ();
};
