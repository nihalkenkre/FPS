// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GrenadeBase.generated.h"

class UCurveFloat;

UCLASS()
class FPS_API AGrenadeBase : public AInteractable
{
	GENERATED_BODY ()
	
public:	
	AGrenadeBase ();

	UPROPERTY (EditAnywhere)
		int Lifetime;

	UPROPERTY (EditAnywhere)
		float BaseDamage;

	UPROPERTY (EditAnywhere)
		float MinimumDamage;

	UPROPERTY (EditAnywhere)
		float OuterRadius;

	UPROPERTY (EditAnywhere)
		float InnerRadius;

	UPROPERTY (EditAnywhere)
		float DamageFalloff;

	UPROPERTY (EditAnywhere)
		UCurveFloat* DamageCurve;

	virtual void PickUp (ACharacter* PlayerCharacter) override;
	virtual void Drop (ACharacter* PlayerCharacter) override;

protected:
	FTimerHandle LifetimeTimerHandle;
};
