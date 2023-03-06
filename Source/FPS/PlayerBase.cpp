// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerBase.h"
#include "FPS.h"

APlayerBase::APlayerBase ()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

/*float APlayerBase::TakeDamage (float DamageAmount, const FDamageEvent& DamageEvent, AController* Instigator, AActor* DamageCauser)
{
	return Super::TakeDamage (DamageAmount, DamageEvent, Instigator, DamageCauser);
}*/

float APlayerBase::TakeBulletDamage (float Damage)
{
	return 0;
}

float APlayerBase::TakeExplosiveDamage (float Damage)
{
	return 0;
}

float APlayerBase::TakeFlashDamage (float Damage, FVector FlashLocation)
{
	return 0;
}

float APlayerBase::TakeSmokeDamage (float Damage)
{
	return 0;
}

void APlayerBase::CheckHealth ()
{
}