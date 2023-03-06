// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "AllEnums.h"
#include "Weapon.generated.h"

class UBoxComponent;
class UCurveVector;
class UCurveFloat;
class USceneComponent;
class UPhysicsHandleComponent;
class UPhysicsConstraintComponent;
class AVRPlayer;
class UAudioComponent;
class UWidgetComponent;
class ABullet;
class ABulletTrail;

UCLASS()
class FPS_API AWeapon : public AInteractable
{
	GENERATED_BODY()
	
public:	
	AWeapon ();

	UPROPERTY (VisibleAnywhere)
		UBoxComponent* SteadyInteractionZone;

	UPROPERTY (VisibleAnywhere)
		UStaticMeshComponent* Magazine;

	UPROPERTY (EditAnywhere)
		EWeaponFireMode FireMode;

	UPROPERTY (EditAnywhere)
		int MaximumBullets;

	UPROPERTY (EditAnywhere)
		int RoundsPerMinute;

	UPROPERTY (EditAnywhere)
		UCurveVector* RecoilCurve;

	UPROPERTY (EditAnywhere)
		UCurveFloat* BulletSpreadCurve;

	UPROPERTY (EditAnywhere)
		UAudioComponent* SingleShotAudio;

	UPROPERTY (EditAnywhere)
		UAudioComponent* ReloadStartAudio;

	UPROPERTY (EditAnywhere)
		UAudioComponent* ReloadEndAudio;

	UPROPERTY (EditAnywhere)
		UAudioComponent* EmptyShotAudio;

	UPROPERTY (EditAnywhere)
		TSubclassOf<ABullet> BulletClass;

	UPROPERTY (EditAnywhere)
		TArray<TSubclassOf<ABulletTrail>> BulletTrailsClass;

	UPROPERTY (EditAnywhere)
		int BulletTrailSpeed;

	void TriggerPressed ();
	void TriggerReleased ();

	void Reload ();

	virtual void PickUp (ACharacter* PlayerCharacter) override;
	virtual void Drop (ACharacter* PlayerCharacter) override;

	virtual void BeginPlay () override;

	void Shoot ();
	void FireBullet ();
	void FireBulletHitScan ();
	void AddRecoil ();
	void GenerateBulletTrail (FTransform MuzzleTransform, FVector TrailDirection);

private:
	float ShootingInterval;

	int NumberOfBullets;
	int NumberOfBulletsFiredCurrentTriggerPress;
	
	bool IsReloading;

	FTimerHandle AutoFireTimerHandle;
};
