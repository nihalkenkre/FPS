// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBase.h"
#include "BOT.generated.h"

class UCameraComponent;
class APlayerManager;
class UBehaviorTree;

UCLASS()
class FPS_API ABOT : public APlayerBase
{
	GENERATED_BODY ()
	
public:	
	ABOT ();

	UPROPERTY (EditDefaultsOnly, BlueprintReadOnly)
		float MaxWalkSpeed;

	UPROPERTY (EditDefaultsOnly, BlueprintReadOnly)
		float MaxRunSpeed;

	UPROPERTY (EditDefaultsOnly, BlueprintReadOnly)
		float MaxWalkSpeedCrouched;

	UPROPERTY (EditDefaultsOnly, BlueprintReadOnly)
		float MaxRunSpeedCrouched;

	UPROPERTY (EditDefaultsOnly)
		UBehaviorTree* BehaviorTree;

	virtual void BeginPlay () override;
	virtual void Tick (float DeltaTime) override;

	virtual float TakeDamage (float DamageAmount, const FDamageEvent& DamageEvent, AController* Instigator, AActor* DamageCauser) override;

	virtual float TakeBulletDamage (float Damage) override;
	virtual float TakeExplosiveDamage (float Damage) override;
	virtual float TakeFlashDamage (float Damage, FVector FlashLocation) override;
	virtual float TakeSmokeDamage (float Damage) override;

	virtual void CheckHealth () override;
	
protected:
	void OnDeathTimerOver ();
};
