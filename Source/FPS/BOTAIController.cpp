// Fill out your copyright notice in the Description page of Project Settings.

#include "BOTAIController.h"

#include "BOT.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ABOTAIController::ABOTAIController ()
{
    PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;

    BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent> (FName ("Behavior Tree"));
    BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent> (FName ("Blackboard"));

    bWantsPlayerState = false;
    bAttachToPawn = true;
}

void ABOTAIController::OnPossess (APawn* InPawn)
{
    Super::OnPossess (InPawn);
}

void ABOTAIController::OnUnPossess ()
{
    Super::OnUnPossess ();
}

void ABOTAIController::BOTKilled ()
{
    /*AFPSGameModeBase* GameMode = Cast<AFPSGameModeBase> (UGameplayStatics::GetGameMode (GetWorld ()));
        
    if (GameMode->IsA (AFPSDMGameMode::StaticClass ()))
    {
        Cast<AFPSDMGameMode> (GameMode)->BOTKilled (this);
    }
    else if (GameMode->IsA (AFPSCTFGameMode::StaticClass ()))
    {
        Cast<AFPSCTFGameMode> (GameMode)->BOTKilled (this);
    }*/
}