// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSPlayerStateBase.h"

AFPSPlayerStateBase::AFPSPlayerStateBase ()
{
    PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;
}