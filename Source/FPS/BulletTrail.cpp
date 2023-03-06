// Fill out your copyright notice in the Description page of Project Settings.

#include "BulletTrail.h"

#include "GameFramework/ProjectileMovementComponent.h"

ABulletTrail::ABulletTrail ()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	TrailMesh = CreateDefaultSubobject <UStaticMeshComponent> (FName ("Trail Mesh"));
	TrailMesh->SetEnableGravity (false);
	TrailMesh->SetSimulatePhysics (false);
	TrailMesh->SetCastShadow (false);
	TrailMesh->SetCollisionProfileName (FName ("BulletTrail"));

	RootComponent = TrailMesh;

	ProjectileMovement = CreateDefaultSubobject <UProjectileMovementComponent> (FName ("Bullet Movement"));

	ProjectileMovement->ProjectileGravityScale = 0.f;

	Lifetime = 2.f;
}

void ABulletTrail::BeginPlay ()
{
	Super::BeginPlay ();

	ProjectileMovement->OnProjectileStop.AddDynamic (this, &ABulletTrail::OnStop);
	GetWorldTimerManager ().SetTimer (LifetimeTimerHandle, this, &ABulletTrail::OnLifetimeOver, Lifetime, false);
}

void ABulletTrail::OnLifetimeOver ()
{
	Destroy ();
}

void ABulletTrail::OnStop (const FHitResult& HitResult)
{
	Destroy ();
}