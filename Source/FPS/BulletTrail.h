// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletTrail.generated.h"

class UStaticMeshComponent;
class UProjectileMovementComponent;

UCLASS()
class FPS_API ABulletTrail : public AActor
{
	GENERATED_BODY ()
	
public:	
	ABulletTrail ();

	UPROPERTY (VisibleAnywhere)
		UStaticMeshComponent* TrailMesh;

	UPROPERTY (VisibleAnywhere)
		UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY (EditAnywhere)
		int Lifetime;

	virtual void BeginPlay () override;

protected:
	UFUNCTION ()
		void OnStop (const FHitResult& HitResult);

	void OnLifetimeOver ();

private:
	FTimerHandle LifetimeTimerHandle;
};
