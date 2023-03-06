// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSGameStateBase.h"

AFPSGameStateBase::AFPSGameStateBase ()
{
    PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;
}