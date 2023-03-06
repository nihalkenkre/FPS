// Fill out your copyright notice in the Description page of Project Settings.

#include "GrenadeBase.h"
#include "VRPlayer.h"
#include "AllEnums.h"

AGrenadeBase::AGrenadeBase ()
{
    Lifetime = 4;

    BaseDamage = 50.f;
    DamageFalloff = 1.f;
    OuterRadius = 1000.f;
    InnerRadius = 20.f;
}

void AGrenadeBase::PickUp (ACharacter* PlayerCharacter)
{
    Super::PickUp (PlayerCharacter);
}

void AGrenadeBase::Drop (ACharacter* PlayerCharacter)
{
    Super::Drop (PlayerCharacter);
}
