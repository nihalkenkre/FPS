// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"

#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Components/WidgetComponent.h"

#include "GameFramework/ProjectileMovementComponent.h"

#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

#include "Kismet/GameplayStatics.h"

#include "Kismet/GameplayStatics.h"
#include "Curves/CurveVector.h"
#include "Curves/CurveFloat.h"

#include "Bullet.h"
#include "BulletTrail.h"
#include "BOT.h"
#include "VRPlayer.h"
#include "PlayerBase.h"

#include "AllDamageTypes.h"

#include "DrawDebugHelpers.h"

AWeapon::AWeapon ()
{
	SteadyInteractionZone = CreateDefaultSubobject<UBoxComponent> (TEXT ("Steady Interaction Zone"));
	SteadyInteractionZone->SetCollisionProfileName (FName ("Trigger"));
	SteadyInteractionZone->SetupAttachment (GetRootComponent ());

	Magazine = CreateDefaultSubobject<UStaticMeshComponent> (TEXT ("Magazine"));
	Magazine->SetupAttachment (GetRootComponent ());
	Magazine->SetCollisionProfileName (FName ("Interactable"));

	SingleShotAudio = CreateDefaultSubobject<UAudioComponent> (TEXT ("Single Shot Audio"));
	ReloadStartAudio = CreateDefaultSubobject<UAudioComponent> (TEXT ("Reload Start Audio"));
	ReloadEndAudio = CreateDefaultSubobject<UAudioComponent> (TEXT ("Reload End Audio"));
	EmptyShotAudio = CreateDefaultSubobject<UAudioComponent> (TEXT ("Empty Shot Audio"));

	SingleShotAudio->bAutoActivate = false;
	ReloadStartAudio->bAutoActivate = false;
	ReloadEndAudio->bAutoActivate = false;
	EmptyShotAudio->bAutoActivate = false;

	FireMode = EWeaponFireMode::Single;
}

void AWeapon::BeginPlay () 
{
	Super::BeginPlay ();

	ShootingInterval = 1.f / ((float)RoundsPerMinute / 60);

	NumberOfBullets = MaximumBullets;
}

void AWeapon::TriggerPressed () 
{
	if (NumberOfBullets > 0) 
	{
		if (FireMode == EWeaponFireMode::Auto) 
		{
			GetWorldTimerManager ().SetTimer (AutoFireTimerHandle, this, &AWeapon::Shoot, ShootingInterval, true, 0.f);
		}
		else
		{
			Shoot ();
		}
	}
}

void AWeapon::TriggerReleased () 
{
	if (FireMode == EWeaponFireMode::Auto)
	{
		GetWorldTimerManager ().ClearTimer (AutoFireTimerHandle);
		NumberOfBulletsFiredCurrentTriggerPress = 0;
	}
}

void AWeapon::Shoot () 
{
	FireBulletHitScan ();
	NumberOfBullets--;
	AddRecoil ();
	SingleShotAudio->Play ();

	NumberOfBulletsFiredCurrentTriggerPress++;

	if (NumberOfBullets == 0) 
	{
		GetWorldTimerManager ().ClearTimer (AutoFireTimerHandle);
		NumberOfBulletsFiredCurrentTriggerPress = 0;
	}
}

void AWeapon::FireBullet ()
{
	if (!BulletClass)
	{
		return;
	}

	float SpreadRand = 0;

	if (BulletSpreadCurve) 
	{
		SpreadRand = BulletSpreadCurve->GetFloatValue (NumberOfBulletsFiredCurrentTriggerPress);
	}

	FVector ActorForwardVector = GetActorForwardVector ();
	FVector BulletDirection = FVector (ActorForwardVector.X, ActorForwardVector.Y + FMath::RandRange (-SpreadRand, SpreadRand), ActorForwardVector.Z + FMath::RandRange (-SpreadRand, SpreadRand));

	FTransform MuzzleTransform = InteractableMesh->GetSocketTransform (FName ("Muzzle"));
	
	ABullet* BulletActor = Cast<ABullet> (UGameplayStatics::BeginDeferredActorSpawnFromClass (GetWorld (), BulletClass, MuzzleTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn, this));
	Cast<UPrimitiveComponent> (BulletActor->GetRootComponent ())->SetPhysicsLinearVelocity (BulletDirection * BulletTrailSpeed);
	
	if (InteractableSubType == EInteractableSubType::AssaultRifle) 
	{
		 BulletActor->TrailMesh->SetWorldScale3D (FVector (FMath::RandRange (1.f, 2.f), FMath::RandRange (1.f, 2.f), FMath::RandRange (1.f, 2.f)));
	}
	else if (InteractableSubType == EInteractableSubType::SMG || InteractableSubType == EInteractableSubType::Pistol)
	{
		BulletActor->TrailMesh->SetWorldScale3D (FVector (FMath::RandRange (1.f, 2.f), FMath::RandRange (1.f, 2.f), FMath::RandRange (1.f, 2.f)));
	}

	UGameplayStatics::FinishSpawningActor (BulletActor, MuzzleTransform);
}

void AWeapon::FireBulletHitScan ()
{
	float SpreadRand = 0;

	if (BulletSpreadCurve)
	{
		SpreadRand = BulletSpreadCurve->GetFloatValue (NumberOfBulletsFiredCurrentTriggerPress);
	}

	FVector ActorForwardVector = GetActorForwardVector ();
	FVector TrailDirection = FVector (ActorForwardVector.X, ActorForwardVector.Y + FMath::RandRange (-SpreadRand, SpreadRand), ActorForwardVector.Z + FMath::RandRange (-SpreadRand, SpreadRand));

	FHitResult HitResult;
	FTransform MuzzleTransform = InteractableMesh->GetSocketTransform (FName ("Muzzle"));
	FVector EndTrace = MuzzleTransform.GetLocation () + (TrailDirection * 10000.f);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor (this);
	QueryParams.AddIgnoredActor (GetOwner ());

	if (GetWorld ()->LineTraceSingleByChannel (HitResult, MuzzleTransform.GetLocation (), EndTrace, ECollisionChannel::ECC_GameTraceChannel4, QueryParams)) 
	{
		AActor* HitActor = Cast<AActor> (HitResult.Actor.Get ());

		if (HitActor)
		{
			DrawDebugLine (GetWorld (), HitResult.ImpactPoint, HitResult.ImpactPoint + (TrailDirection * 50), FColor::Red, true, 2.f, 0, 1.f);
			if (HitActor->ActorHasTag (FName ("Target"))) 
			{
				Cast<UPrimitiveComponent> (HitActor->GetRootComponent ())->AddForceAtLocation (TrailDirection * 1000000, HitResult.ImpactPoint, NAME_None);
			}

			if (HitActor->IsA (APlayerBase::StaticClass ())) 
			{
				APlayerBase* Player = Cast<APlayerBase> (HitActor);
				FDamageEvent DamageEvent (UBulletDamageType::StaticClass ());

				UGameplayStatics::ApplyPointDamage (Player, 50, -TrailDirection, HitResult, GetInstigatorController (), this, UBulletDamageType::StaticClass ());
			}
		}
	}
	
	GenerateBulletTrail (MuzzleTransform, TrailDirection);
}

void AWeapon::GenerateBulletTrail (FTransform MuzzleTransform, FVector TrailDirection)
{
	if (BulletTrailsClass.Num () == 0) 
	{
		return;
	}

	int BulletTrailIndex = FMath::RandRange (0, BulletTrailsClass.Num () - 1);

	if (!BulletTrailsClass[BulletTrailIndex]) 
	{
		return;
	}

	bool IsGenerationNeeded = FMath::RandRange (0, 1) == 1 ? true : false;

	if (!IsGenerationNeeded) 
	{
		return;
	}

	ABulletTrail* BulletTrailActor = Cast<ABulletTrail> (UGameplayStatics::BeginDeferredActorSpawnFromClass (GetWorld (), BulletTrailsClass[BulletTrailIndex], MuzzleTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn, this));

	if (InteractableSubType == EInteractableSubType::AssaultRifle) 
	{
		BulletTrailActor->TrailMesh->SetWorldScale3D (FVector (FMath::RandRange (0.5, 2.f), FMath::RandRange (0.5f, 1.f), FMath::RandRange (0.5f, 1.f)));
		BulletTrailActor->ProjectileMovement->InitialSpeed = BulletTrailSpeed;

	}
	else if (InteractableSubType == EInteractableSubType::SMG || InteractableSubType == EInteractableSubType::Pistol) 
	{
		BulletTrailActor->TrailMesh->SetWorldScale3D (FVector (FMath::RandRange (0.5f, 0.75f), FMath::RandRange (0.5f, 1.f), FMath::RandRange (0.5f, 1.f)));
		BulletTrailActor->ProjectileMovement->InitialSpeed = BulletTrailSpeed;
	}

	UGameplayStatics::FinishSpawningActor (BulletTrailActor, MuzzleTransform);
}

void AWeapon::AddRecoil () 
{
	if (!RecoilCurve) 
	{
		return;
	}
	
	FVector ForceTorqueRand = RecoilCurve->GetVectorValue (NumberOfBulletsFiredCurrentTriggerPress);
	FVector RandOffset (FMath::RandRange (-ForceTorqueRand.Z, ForceTorqueRand.Z), FMath::RandRange (-ForceTorqueRand.Z, ForceTorqueRand.Z), FMath::RandRange (-ForceTorqueRand.Z, ForceTorqueRand.Z));

	InteractableMesh->AddForceAtLocation ((ForceTorqueRand.X) * (-GetActorForwardVector () + RandOffset), InteractableMesh->GetSocketLocation (FName ("Muzzle")));
	InteractableMesh->AddTorqueInDegrees ((ForceTorqueRand.Y) * (-GetActorRightVector () + RandOffset), NAME_None, true);
}

void AWeapon::PickUp (ACharacter* PlayerCharacter)
{
	Super::PickUp (PlayerCharacter);

	if (InteractableType == EInteractableType::PrimaryWeapon)
	{
		AttachmentPoint = Cast<AVRPlayer> (PlayerCharacter)->PrimaryParent;
	}
	else if (InteractableType == EInteractableType::SecondaryWeapon)
	{
		AttachmentPoint = Cast<AVRPlayer> (PlayerCharacter)->SecondaryParent;
	}
}

void AWeapon::Drop (ACharacter* PlayerCharacter)
{
	Super::Drop (PlayerCharacter);
}

void AWeapon::Reload ()
{
	NumberOfBullets = MaximumBullets;
}