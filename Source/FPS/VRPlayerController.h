// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "AllEnums.h"

#include "VRPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam (FOnTimerUpdate, float, TimerRemaining);

class AVRPlayer;
class AFPSPlayerStateBase;
class UWidgetComponent;
class APlayerManager;

UCLASS()
class FPS_API AVRPlayerController : public APlayerController
{
	GENERATED_BODY ()

public:
	AVRPlayerController ();

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly)
		UWidgetComponent* TimerWidget;

	void SetTeamType (ETeamType Type);
	void SetMatchState (EMatchState State);
	void SetPlayerPlayState (EPlayerPlayState State);

	virtual void BeginPlay () override;
	virtual void SetupInputComponent () override;

	virtual void OnPossess (APawn* InPawn) override;	
	virtual void OnUnPossess () override;
	
protected:
	void LeftThumbstickUpDown (float AxisValue);
	void LeftThumbstickLeftRight (float AxisValue);

	void RightThumbstickUpDown (float AxisValue);
	void RightThumbstickLeftRight (float AxisValue);

	void ThumbstickPressed ();

	void LeftXPressed ();
	void RightAPressed ();

	void LeftGripPressed ();
	void LeftGripReleased ();

	void RightGripPressed ();
	void RightGripReleased ();

	void LeftTriggerPressed ();
	void LeftTriggerReleased ();

	void RightTriggerPressed ();
	void RightTriggerReleased ();

	void MenuPressed ();

	void LeftThumbstickFull ();
	void RightThumbstickFull ();

	void LeftThumbstickNotFull ();
	void RightThumbstickNotFull ();

	void WarmupTimer ();

private:
	EMatchState MatchState;

	AVRPlayer* Player;
	AFPSPlayerStateBase* FPSPlayerState;

	float WarmupTimeElapsed;

	FTimerHandle WarmupTimerHandle;
};
