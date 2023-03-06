// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AllEnums.h"
#include "Bullet.generated.h"

class UStaticComponent;
class UParticleSystemComponent;

UCLASS()
class FPS_API ABullet : public AActor
{
	GENERATED_BODY ()
	
public:	
	ABullet ();

	UPROPERTY (VisibleAnywhere)
		UStaticMeshComponent* BulletMesh;

	UPROPERTY (VisibleAnywhere)
		UStaticMeshComponent* TrailMesh;

	UPROPERTY (EditAnywhere)
		float BulletLifetime;

	virtual void BeginPlay () override;
	virtual void EndPlay (const EEndPlayReason::Type EndPlayReason) override;
	
protected:
	UFUNCTION ()
		void OnHit (UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult);

	UFUNCTION ()
		void OnLifetimeOver ();

private:
	FTimerHandle LifetimeTimerHandle;
};
