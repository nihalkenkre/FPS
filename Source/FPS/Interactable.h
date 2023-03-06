// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AllEnums.h"
#include "Interactable.generated.h"

class UBoxComponent;
class USceneComponent;
class ACharacter;

UCLASS()
class FPS_API AInteractable : public AActor
{
	GENERATED_BODY ()

public:
	AInteractable ();

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly)
		USkeletalMeshComponent* InteractableMesh;

	UPROPERTY (VisibleAnywhere)
		UBoxComponent* GripInteractionZone;

	UPROPERTY (EditAnywhere)
		EInteractableType InteractableType;

	UPROPERTY (EditAnywhere)
		EInteractableSubType InteractableSubType;

	USceneComponent* AttachmentPoint;

	bool IsHeld;

	virtual void PickUp (ACharacter* PlayerCharacter);
	virtual void Drop (ACharacter* PlayerCharacter);

private:
	float ThresholdDistance;
};