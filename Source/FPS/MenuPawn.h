// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MenuPawn.generated.h"

class UInputComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UWidgetInteractionComponent;

UCLASS()
class FPS_API AMenuPawn : public APawn
{
	GENERATED_BODY ()

public:
	AMenuPawn ();

	UPROPERTY (VisibleAnywhere)
		USceneComponent* VRScene;

	UPROPERTY (VisibleAnywhere)
		UCameraComponent* Camera;

	UPROPERTY (VisibleAnywhere)
		UMotionControllerComponent* LeftMotionController;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly)
		UWidgetInteractionComponent* LeftWidgetInteractionComponent;

	UPROPERTY (VisibleAnywhere)
		UMotionControllerComponent* RightMotionController;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly)
		UWidgetInteractionComponent* RightWidgetInteractionComponent;

	UPROPERTY (VisibleAnywhere)
		USkeletalMeshComponent* LeftHandMesh;

	UPROPERTY (VisibleAnywhere)
		USkeletalMeshComponent* RightHandMesh;

	virtual void BeginPlay () override;
	virtual void SetupPlayerInputComponent (UInputComponent* PlayerInputComponent) override;
	virtual void Tick (float DeltaTime) override;
	
protected:
	void LeftTriggerPressed ();
	void LeftTriggerReleased ();

	void RightTriggerPressed ();
	void RightTriggerReleased ();
};
