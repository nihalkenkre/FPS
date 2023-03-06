// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerBase.generated.h"

class APlayerManager;
class UCameraComponent;

UCLASS()
class FPS_API APlayerBase : public ACharacter
{
	GENERATED_BODY ()

public:
	APlayerBase ();

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly)
		UCameraComponent* Camera;

//	virtual float TakeDamage (float DamageAmount, const FDamageEvent& DamageEvent, AController* Instigator, AActor* DamageCauser) override;

	virtual float TakeBulletDamage (float Damage);
	virtual float TakeExplosiveDamage (float Damage);
	virtual float TakeFlashDamage (float Damage, FVector FlashLocation);
	virtual float TakeSmokeDamage (float Damage);
	virtual void CheckHealth ();

	APlayerManager* PlayerManager;

protected:
	UPROPERTY (VisibleAnywhere, BlueprintReadOnly)
		float Health;
};
