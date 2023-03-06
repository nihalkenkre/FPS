// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "AllEnums.h"

#include "BOTAIController.generated.h"

class UBehaviorTreeComponent;
class UBlackboardComponent;

UCLASS()
class FPS_API ABOTAIController : public AAIController
{
	GENERATED_BODY ()
	
public:
	ABOTAIController ();

	virtual void OnPossess (APawn* InPawn) override;
	virtual void OnUnPossess () override;

	void BOTKilled ();

	UBehaviorTreeComponent* BehaviorTreeComponent;
	UBlackboardComponent* BlackboardComponent;
};
