// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBase.h"
#include "Components/TimelineComponent.h"

#include "AllEnums.h"

#include "VRPlayer.generated.h"

class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class USkeletalMeshComponent;
class UBoxComponent;
class UPhysicsHandleComponent;

class UInputComponent;
class UMaterialInstanceDynamic;
class UPostProcessComponent;
class UWidgetComponent;
class UWidgetInteractionComponent;

class ARifleBase;
class AWeapon;
class AGrenadeBag;
class AGrenadeBase;
class AInteractable;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam (FOnFlashed, float, Intensity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam (FOnWalling, float, Intensity);

UCLASS()
class FPS_API AVRPlayer : public APlayerBase
{
	GENERATED_BODY ()

public:
	AVRPlayer ();

	UPROPERTY (VisibleAnywhere)
		USceneComponent* VRScene;

	UPROPERTY (VisibleAnywhere, BlueprintReadWrite)
		UWidgetComponent* MenuWidget;

	UPROPERTY (VisibleAnywhere)
		UBoxComponent* CameraOverlap;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly)
		UMotionControllerComponent* LeftMotionController;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly)
		UWidgetInteractionComponent* LeftWidgetInteractionComponent;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly)
		UMotionControllerComponent* RightMotionController;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly)
		UWidgetInteractionComponent* RightWidgetInteractionComponent;

	UPROPERTY (VisibleAnywhere)
		USkeletalMeshComponent* LeftHandMesh;

	UPROPERTY (VisibleAnywhere)
		USkeletalMeshComponent* RightHandMesh;

	UPROPERTY (VisibleAnywhere)
		UBoxComponent* RightHandInteractionZone;

	UPROPERTY (VisibleAnywhere)
		UBoxComponent* LeftHandInteractionZone;

	UPROPERTY (VisibleAnywhere)
		UPhysicsHandleComponent* LeftPhysicsHandle;

	UPROPERTY (VisibleAnywhere)
		UPhysicsHandleComponent* RightPhysicsHandle;

	UPROPERTY (VisibleAnywhere)
		USceneComponent* PrimaryParent;

	UPROPERTY (VisibleAnywhere)
		USceneComponent* SecondaryParent;

	UPROPERTY (VisibleAnywhere)
		USceneComponent* FlashParent;

	UPROPERTY (VisibleAnywhere)
		USceneComponent* ExplosiveParent;

	UPROPERTY (VisibleAnywhere)
		USceneComponent* SmokeParent;

	UPROPERTY (VisibleAnywhere, BlueprintReadWrite)
		UPostProcessComponent* PostProcess;

	UPROPERTY (VisibleAnywhere, BlueprintReadWrite)
		UMaterialInstanceDynamic* FlashEffectMaterial;

	UPROPERTY (VisibleAnywhere, BlueprintReadWrite)
		UMaterialInstanceDynamic* WallingMaterial;

	UPROPERTY (BlueprintAssignable)
		FOnFlashed OnFlashed;

	UPROPERTY (BlueprintAssignable)
		FOnWalling OnWalling;

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

	void LeftThumbstickFull ();
	void LeftThumbstickNotFull ();
	
	void RightThumbstickFull ();
	void RightThumbstickNotFull ();

	void ToggleMenu ();

	virtual void BeginPlay () override;
	virtual void Tick (float DeltaTime) override;
	virtual void EndPlay (const EEndPlayReason::Type EndPlayReason) override;

	//virtual float TakeDamage (float DamageAmount, const FDamageEvent& DamageEvent, AController* Instigator, AActor* DamageCauser) override;

	virtual float TakeBulletDamage (float Damage) override;
	virtual float TakeExplosiveDamage (float Damage) override;
	virtual float TakeFlashDamage (float Damage, FVector FlashLocation) override;
	virtual float TakeSmokeDamage (float Damage) override;

	virtual void CheckHealth () override;

	UFUNCTION (BlueprintCallable)
		void SpawnInteractableOfClass (TSubclassOf <AInteractable> InteractableClass, EInteractableType Type, EInteractableSubType SubType);

protected:
	UFUNCTION ()
		void OnLeftInteractionZoneOverlapBegin (UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool IsFromSweep, const FHitResult& SweepResult);

	UFUNCTION ()
		void OnLeftInteractionZoneOverlapEnd (UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION ()
		void OnRightInteractionZoneOverlapBegin (UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool IsFromSweep, const FHitResult& SweepResult);

	UFUNCTION ()
		void OnRightInteractionZoneOverlapEnd (UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION ()
		void OnCameraOverlapBegin (UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool IsFromSweep, const FHitResult& SweepResult);

	UFUNCTION ()
		void OnCameraOverlapEnd (UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION ()
		void TakeAnyDamage (AActor* DamagedActor, float DamageAmount, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION ()
		void TakePointDamage (AActor* DamagedActor, float DamageAmount, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* HitComponent, FName BoneName, FVector HitDirection, const UDamageType* DamageType, AActor* DamageCauser);

	UFUNCTION ()
		void TakeRadialDamage (AActor* DamagedActor, float DamageAmount, const UDamageType* DamageType, FVector Origin, FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser);

	void UpdateMovementSpeed ();
	void UpdateClosestLeftInteractable ();
	void UpdateClosestRightInteractable ();

	void OnDeathTimerOver ();
	void DrawInteractionTargetLines ();

private:
	bool IsLeftHandHovering;
	bool IsRightHandHovering;

	bool IsLeftHandHolding;
	bool IsRightHandHolding;

	bool IsFlipped180;

	bool IsRightThumbStickFull;
	bool IsLeftThumbStickFull;

	float MaxPlayerWalkSpeed;
	float MaxPlayerRunSpeed;

	float MaxPlayerCrouchWalkSpeed;
	float MaxPlayerCrouchRunSpeed;

	AInteractable* LeftHandHoldingActor;
	AInteractable* RightHandHoldingActor;

	FTimeline MovementSlowDownTimeline;
	FTimerHandle MovementSlowDownTimerHandle;

	TArray <UPrimitiveComponent*> OverlappedLeftInteractableComponents;
	TArray <UPrimitiveComponent*> OverlappedRightInteractableComponents;
	
	UPrimitiveComponent* ClosestLeftInteractableComponent;
	UPrimitiveComponent* ClosestRightInteractableComponent;

	UPrimitiveComponent* LeftHandHoldingComponent;
	UPrimitiveComponent* RightHandHoldingComponent;

	EInteractionType PlayerInteractionType;
	
	EInteractionType RightHandInteractionType;
	EInteractionType LeftHandInteractionType;
};
