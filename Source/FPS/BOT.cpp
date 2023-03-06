// Fill out your copyright notice in the Description page of Project Settings.

#include "BOT.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

#include "PlayerManager.h"
#include "BOTAIController.h"
#include "FPSGameStateBase.h"
#include "FPSDMGameMode.h"
#include "FPSCTFGameMode.h"
#include "FPS.h"

ABOT::ABOT ()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	AIControllerClass = ABOTAIController::StaticClass ();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;	

	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	Camera = CreateDefaultSubobject <UCameraComponent> (FName ("Camera"));

	Cast<UPrimitiveComponent> (GetRootComponent ())->SetCollisionProfileName (FName ("PlayerCapsule"));
	Cast<UPrimitiveComponent> (GetMesh ())->SetCollisionProfileName (FName ("PlayerMesh"));

	MaxWalkSpeed = 150.f;
	MaxRunSpeed = 300.f;

	MaxWalkSpeedCrouched = 75.f;
	MaxRunSpeedCrouched = 150.f;

	GetCharacterMovement ()->MaxWalkSpeed = MaxWalkSpeed;
	GetCharacterMovement ()->MaxWalkSpeedCrouched = MaxWalkSpeedCrouched;

	Health = 100;
}

void ABOT::BeginPlay ()
{
	Super::BeginPlay ();

	Camera->AttachToComponent (GetMesh (), FAttachmentTransformRules (EAttachmentRule::SnapToTarget, true), FName ("HeadSocket"));
}

void ABOT::Tick (float DeltaTime)
{
	Super::Tick (DeltaTime);

	/*if (Health > 0)
	{
		Health -= FMath::RandRange (0.2f, 1.f);
		CheckHealth ();
	}*/
}

float ABOT::TakeDamage (float DamageAmount, const FDamageEvent& DamageEvent, AController* Instigator, AActor* DamageCauser)
{
	if (Health > 0)
	{
		Health -= Super::TakeDamage (DamageAmount, DamageEvent, Instigator, DamageCauser);
	}
	
	//GEngine->AddOnScreenDebugMessage (++DebugLogCounter, 2.f, FColor::Red, TEXT ("BOT Take Damage"));
	//GEngine->AddOnScreenDebugMessage (++DebugLogCounter, 2.f, FColor::Red, FString::SanitizeFloat (Health));

	CheckHealth ();

	return DamageAmount;
}

float ABOT::TakeBulletDamage (float Damage)
{
	Super::TakeBulletDamage (Damage);

	Health -= Damage;

	CheckHealth ();

	return 0;
}

float ABOT::TakeExplosiveDamage (float Damage)
{
	Super::TakeExplosiveDamage (Damage);

	Health -= Damage;

	CheckHealth ();

	return 0;
}

float ABOT::TakeFlashDamage (float Damage, FVector FlashLocation)
{
	Super::TakeFlashDamage (Damage, FlashLocation);

	CheckHealth ();

	return 0;
}

float ABOT::TakeSmokeDamage (float Damage)
{
	Super::TakeSmokeDamage (Damage);
	
	CheckHealth ();

	return 0;
}

void ABOT::CheckHealth ()
{
	Super::CheckHealth ();
	
	if (Health <= 0) 
	{
		GetMesh ()->SetSimulatePhysics (true);
		GetMesh ()->SetCollisionProfileName (FName ("PhysicsActor"));

		Cast<UPrimitiveComponent> (GetRootComponent ())->SetCollisionProfileName (FName ("DeadBody"));
		
		AFPSGameModeBase* GameMode = Cast<AFPSGameModeBase> (UGameplayStatics::GetGameMode (GetWorld ()));
        
		if (GameMode->IsA (AFPSDMGameMode::StaticClass ()))
		{
			Cast<AFPSDMGameMode> (GameMode)->BOTKilled (this);
		}
		else if (GameMode->IsA (AFPSCTFGameMode::StaticClass ()))
		{
	//        Cast<AFPSCTFGameMode> (GameMode)->BOTKilled (this);
		}
	}
}

void ABOT::OnDeathTimerOver ()
{
	Destroy ();
}