// Fill out your copyright notice in the Description page of Project Settings.

#include "Bullet.h"

#include "Kismet/GameplayStatics.h"

#include "BOT.h"

ABullet::ABullet ()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent> (FName ("Bullet"));
	BulletMesh->SetNotifyRigidBodyCollision (true);
	BulletMesh->SetSimulatePhysics (true);
	BulletMesh->GetBodyInstance (NAME_None, false)->bUseCCD = true;

	RootComponent = BulletMesh;

	TrailMesh = CreateDefaultSubobject<UStaticMeshComponent> (FName ("Trail Mesh"));
	TrailMesh->SetupAttachment (GetRootComponent ());
	TrailMesh->SetCollisionProfileName (FName ("NoCollision"));
	TrailMesh->SetCastShadow (false);

	BulletLifetime = 4.f;
}

void ABullet::BeginPlay ()
{
	Super::BeginPlay ();

	BulletMesh->OnComponentHit.AddDynamic (this, &ABullet::OnHit);

	GetWorldTimerManager ().SetTimer (LifetimeTimerHandle, this, &ABullet::OnLifetimeOver, BulletLifetime, false);
}

void ABullet::EndPlay (const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay (EndPlayReason);

	BulletMesh->OnComponentHit.RemoveAll (this);

	GetWorld ()->GetTimerManager ().ClearAllTimersForObject (this);
}

void ABullet::OnHit (UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	FVector Velocity = HitComp->GetPhysicsLinearVelocity ();
	Velocity.Normalize ();

	if (OtherActor->ActorHasTag (FName ("Target"))) 
	{
		OtherComp->AddForceAtLocation (HitComp->GetPhysicsLinearVelocity () * 100, HitResult.ImpactPoint);
	}

	if (OtherActor->IsA (ABOT::StaticClass ())) 
	{
		FHitResult HitInfo;

		UGameplayStatics::ApplyPointDamage (OtherActor, 50, Velocity, HitInfo, UGameplayStatics::GetPlayerController (this, 0), this, UDamageType::StaticClass ());
	}

	Destroy ();
}

void ABullet::OnLifetimeOver ()
{
	Destroy ();
}