// Fill out your copyright notice in the Description page of Project Settings.

#include "VRPlayer.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "HeadMountedDisplay/Public/HeadMountedDisplayFunctionLibrary.h"

#include "Components/BoxComponent.h"
#include "Components/PostProcessComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "Components/LineBatchComponent.h"

#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Curves/CurveVector.h"

#include "Blueprint/UserWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "Weapon.h"
#include "FlashGrenade.h"
#include "ExplosiveGrenade.h"
#include "SmokeGrenade.h"
#include "PlayerManager.h"
#include "VRPlayerController.h"
#include "FPSGameStateBase.h"
#include "FPSPlayerStateBase.h"
#include "FPS.h"

AVRPlayer::AVRPlayer () 
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = nullptr;

	Cast<UPrimitiveComponent> (GetRootComponent ())->SetCollisionProfileName (FName ("PlayerCapsule"));
	Cast<UPrimitiveComponent> (GetMesh ())->SetCollisionProfileName (FName ("PlayerMesh"));

	VRScene = CreateDefaultSubobject<USceneComponent> (FName ("VR Scene"));
	VRScene->SetupAttachment (GetRootComponent ());

	Camera = CreateDefaultSubobject<UCameraComponent> (FName ("VR Camera"));
	Camera->SetupAttachment (VRScene);

	CameraOverlap = CreateDefaultSubobject<UBoxComponent> (FName ("Camera Overlap"));
	CameraOverlap->SetCollisionProfileName (FName ("Trigger"));
	CameraOverlap->SetupAttachment (Camera);	

	LeftMotionController = CreateDefaultSubobject<UMotionControllerComponent> (FName ("Left Motion Controller"));
	LeftMotionController->MotionSource = FName ("Left");
	LeftMotionController->bDisplayDeviceModel = true;
	LeftMotionController->DisplayModelSource = FName ("OculusHMD");
	LeftMotionController->SetupAttachment (VRScene);
	LeftMotionController->SetCollisionProfileName (FName ("NoCollision"));

	LeftHandMesh = CreateDefaultSubobject<USkeletalMeshComponent> (FName ("Left Hand Mesh"));
	LeftHandMesh->SetupAttachment (LeftMotionController);

	LeftWidgetInteractionComponent = CreateDefaultSubobject<UWidgetInteractionComponent> (FName ("Left Widget Interaction"));
	LeftWidgetInteractionComponent->SetupAttachment (LeftMotionController);

	LeftHandInteractionZone = CreateDefaultSubobject<UBoxComponent> (FName ("Left Hand Interaction"));
	LeftHandInteractionZone->SetCollisionProfileName (FName ("Trigger"));
	LeftHandInteractionZone->SetupAttachment (LeftMotionController);

	RightMotionController = CreateDefaultSubobject<UMotionControllerComponent> (TEXT ("Right Motion Controller"));
	RightMotionController->MotionSource = FName ("Right");
	RightMotionController->bDisplayDeviceModel = true;
	RightMotionController->DisplayModelSource = FName ("OculusHMD");
	RightMotionController->SetupAttachment (VRScene);
	RightMotionController->SetCollisionProfileName (FName ("NoCollision"));

	RightHandMesh = CreateDefaultSubobject<USkeletalMeshComponent> (FName ("Right Hand Mesh"));
	RightHandMesh->SetupAttachment (RightMotionController);

	RightWidgetInteractionComponent = CreateDefaultSubobject<UWidgetInteractionComponent> (FName ("Right Widget Interaction"));
	RightWidgetInteractionComponent->SetupAttachment (RightMotionController);

	RightHandInteractionZone = CreateDefaultSubobject<UBoxComponent> (FName ("Right Hand Interaction"));
	RightHandInteractionZone->SetCollisionProfileName (FName ("Trigger"));
	RightHandInteractionZone->SetupAttachment (RightMotionController);

	MenuWidget = CreateDefaultSubobject <UWidgetComponent> (FName ("Weapons Widget"));
	MenuWidget->bOnlyOwnerSee = true;
    MenuWidget->SetupAttachment (LeftMotionController);

	LeftPhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent> (FName ("Left Physics Handle"));
	RightPhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent> (FName ("Right Physics Handle"));

	PrimaryParent = CreateDefaultSubobject<USceneComponent> (FName ("Primary Parent"));
	PrimaryParent->SetupAttachment (GetMesh ());

	SecondaryParent = CreateDefaultSubobject<USceneComponent> (FName ("Secondary Parent"));
	SecondaryParent->SetupAttachment (GetMesh ());

	FlashParent = CreateDefaultSubobject<USceneComponent> (FName ("Flash Parent"));
	FlashParent->SetupAttachment (GetMesh ());

	ExplosiveParent = CreateDefaultSubobject<USceneComponent> (FName ("Explosive Parent"));
	ExplosiveParent->SetupAttachment (GetMesh ());

	SmokeParent = CreateDefaultSubobject<USceneComponent> (FName ("Smoke Parent"));
	SmokeParent->SetupAttachment (GetMesh ());

	PostProcess = CreateDefaultSubobject <UPostProcessComponent> (FName ("Post Process"));

	MaxPlayerWalkSpeed = 100.f;
	MaxPlayerRunSpeed = 200.f;

	MaxPlayerCrouchWalkSpeed = 50.f;
	MaxPlayerCrouchRunSpeed = 100.f;

	AIControllerClass = nullptr;

	GetCharacterMovement ()->MaxWalkSpeed = MaxPlayerWalkSpeed;
	GetCharacterMovement ()->MaxWalkSpeedCrouched = MaxPlayerCrouchWalkSpeed;

	IsLeftHandHovering = false;
	IsRightHandHovering = false;

	IsFlipped180 = false;

	Health = 100;
}

void AVRPlayer::BeginPlay () 
{
	Super::BeginPlay ();

	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin (EHMDTrackingOrigin::Floor);

	LeftHandInteractionZone->OnComponentBeginOverlap.AddDynamic (this, &AVRPlayer::OnLeftInteractionZoneOverlapBegin);
	LeftHandInteractionZone->OnComponentEndOverlap.AddDynamic (this, &AVRPlayer::OnLeftInteractionZoneOverlapEnd);

	RightHandInteractionZone->OnComponentBeginOverlap.AddDynamic (this, &AVRPlayer::OnRightInteractionZoneOverlapBegin);
	RightHandInteractionZone->OnComponentEndOverlap.AddDynamic (this, &AVRPlayer::OnRightInteractionZoneOverlapEnd);

	CameraOverlap->OnComponentBeginOverlap.AddDynamic (this, &AVRPlayer::OnCameraOverlapBegin);
	CameraOverlap->OnComponentEndOverlap.AddDynamic (this, &AVRPlayer::OnCameraOverlapEnd);

	OnTakeAnyDamage.AddDynamic (this, &AVRPlayer::TakeAnyDamage);
	OnTakePointDamage.AddDynamic (this, &AVRPlayer::TakePointDamage);
	OnTakeRadialDamage.AddDynamic (this, &AVRPlayer::TakeRadialDamage);
}

void AVRPlayer::Tick (float DeltaTime) 
{
	Super::Tick (DeltaTime);

	FVector LeftMotionControllerLocation = LeftMotionController->GetComponentLocation ();
	FVector RightMotionControllerLocation = RightMotionController->GetComponentLocation ();

	LeftPhysicsHandle->SetTargetLocationAndRotation (LeftMotionControllerLocation, LeftMotionController->GetComponentRotation ());
	RightPhysicsHandle->SetTargetLocationAndRotation (RightMotionControllerLocation, RightMotionController->GetComponentRotation ());

	if (PlayerInteractionType == EInteractionType::UI)
	{
		DrawInteractionTargetLines ();
	}

	FVector MeshLocation = Camera->GetRelativeLocation () * FVector (1, 1, 0);
	MeshLocation.Z -=88.f;
	GetMesh ()->SetRelativeLocation (MeshLocation);
}

void AVRPlayer::DrawInteractionTargetLines ()
{
	FHitResult HitResult;

	FVector RightEndLocation = RightMotionController->GetComponentLocation () + (RightMotionController->GetForwardVector () * RightWidgetInteractionComponent->InteractionDistance);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor (this);

	if (GetWorld ()->LineTraceSingleByChannel (HitResult, RightMotionController->GetComponentLocation (), RightEndLocation, ECollisionChannel::ECC_Visibility, QueryParams))
	{
		GetWorld ()->LineBatcher->DrawLine (RightMotionController->GetComponentLocation (), HitResult.ImpactPoint, FLinearColor::White, 0, 0.5f);
	}
	else
	{
		GetWorld ()->LineBatcher->DrawLine (RightMotionController->GetComponentLocation (), RightEndLocation, FLinearColor::White, 0, 0.5f);
	}
}

void AVRPlayer::EndPlay (const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay (EndPlayReason);

	LeftHandInteractionZone->OnComponentBeginOverlap.RemoveAll (this);
	LeftHandInteractionZone->OnComponentEndOverlap.RemoveAll (this);

	RightHandInteractionZone->OnComponentBeginOverlap.RemoveAll (this);
	RightHandInteractionZone->OnComponentEndOverlap.RemoveAll (this);
}

void AVRPlayer::OnCameraOverlapBegin (UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool IsFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag (FName ("WallInside")))
	{
		OnWalling.Broadcast (0.f);
	}
}

void AVRPlayer::OnCameraOverlapEnd (UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag (FName ("WallInside")))
	{
		OnWalling.Broadcast (1.f);
	}
}

void AVRPlayer::OnLeftInteractionZoneOverlapBegin (UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool IsFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this)
	{
		return;
	}

	if (OtherActor->IsA (AInteractable::StaticClass ()))
	{
		OverlappedLeftInteractableComponents.Add (OtherComp);
			
		UpdateClosestLeftInteractable ();
	}
}

void AVRPlayer::OnLeftInteractionZoneOverlapEnd (UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) 
{
	if (OtherActor == this)
	{
		return;
	}

	if (OtherActor->IsA (AInteractable::StaticClass ()))
	{
		OverlappedLeftInteractableComponents.Remove (OtherComp);
		ClosestLeftInteractableComponent = nullptr;

		UpdateClosestLeftInteractable ();
	}
}

void AVRPlayer::OnRightInteractionZoneOverlapBegin (UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool IsFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this)
	{
		return;
	}

	if (OtherActor->IsA (AInteractable::StaticClass ()))
	{
		OverlappedRightInteractableComponents.Add (OtherComp);
			
		UpdateClosestRightInteractable ();
	}
}

void AVRPlayer::OnRightInteractionZoneOverlapEnd (UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == this)
	{
		return;
	}

	if (OtherActor->IsA (AInteractable::StaticClass ()))
	{
		OverlappedRightInteractableComponents.Remove (OtherComp);
		ClosestRightInteractableComponent = nullptr;

		UpdateClosestRightInteractable ();
	}
}

void AVRPlayer::UpdateClosestLeftInteractable ()
{
	FVector LeftMotionControllerLocation = LeftMotionController->GetComponentLocation ();

	float ShortestDistance = TNumericLimits<float>::Max ();

	for (int i = 0; i < OverlappedLeftInteractableComponents.Num (); i++)
	{
		FVector Direction; float Length;
		(LeftMotionControllerLocation - OverlappedLeftInteractableComponents[i]->GetComponentLocation ()).ToDirectionAndLength (Direction, Length);

		if (Length < ShortestDistance)
		{
			ShortestDistance = Length;
			ClosestLeftInteractableComponent = OverlappedLeftInteractableComponents[i];
		}
	}
}

void AVRPlayer::UpdateClosestRightInteractable ()
{
	FVector RightMotionControllerLocation = RightMotionController->GetComponentLocation ();

	float ShortestDistance = TNumericLimits<float>::Max ();

	for (int i = 0; i < OverlappedRightInteractableComponents.Num (); i++)
	{
		FVector Direction; float Length;
		(RightMotionControllerLocation - OverlappedRightInteractableComponents[i]->GetComponentLocation ()).ToDirectionAndLength (Direction, Length);

		if (Length < ShortestDistance)
		{
			ShortestDistance = Length;
			ClosestRightInteractableComponent = OverlappedRightInteractableComponents[i];
		}
	}
}

void AVRPlayer::LeftGripPressed ()
{
	if (ClosestLeftInteractableComponent == nullptr)
	{
		return;
	}

	if (PlayerInteractionType == EInteractionType::RealWorld)
	{
		UPrimitiveComponent* InteractableRootComponent = Cast<UPrimitiveComponent> (ClosestLeftInteractableComponent->GetOwner ()->GetRootComponent ());

		FName HoldingPointSocketName;
		FName PhysicsHandlePointSocketName;

		AActor* Actor = ClosestLeftInteractableComponent->GetOwner ();
		
		if (Actor->IsA (AWeapon::StaticClass ()))
		{
			AWeapon* WeaponActor = Cast<AWeapon> (Actor);

			if (WeaponActor->GripInteractionZone == ClosestLeftInteractableComponent)
			{
				HoldingPointSocketName = FName ("Grip Left Interaction Point");
				PhysicsHandlePointSocketName = FName ("Grip Physics Handle Point");
			}
			else if (WeaponActor->SteadyInteractionZone == ClosestLeftInteractableComponent)
			{
				if (WeaponActor->InteractableType == EInteractableType::SecondaryWeapon)
				{
					if (WeaponActor->IsHeld)
					{
						HoldingPointSocketName = FName ("Steady Left Interaction Point");
						PhysicsHandlePointSocketName = FName ("Steady Physics Handle Point");
					}
					else
					{
						ClosestLeftInteractableComponent = WeaponActor->GripInteractionZone;
						HoldingPointSocketName = FName ("Grip Left Interaction Point");
						PhysicsHandlePointSocketName = FName ("Grip Physics Handle Point");
					}
				}
				else
				{
					HoldingPointSocketName = FName ("Steady Left Interaction Point");
					PhysicsHandlePointSocketName = FName ("Steady Physics Handle Point");
				}
			}
		}
		else if (Actor->IsA (AGrenadeBase::StaticClass ()))
		{
			AGrenadeBase* GrenadeActor = Cast <AGrenadeBase> (Actor);

			if (GrenadeActor->GripInteractionZone == ClosestLeftInteractableComponent)
			{
				HoldingPointSocketName = FName ("Grip Left Interaction Point");
				PhysicsHandlePointSocketName = FName ("Grip Physics Handle Point");
			}
		}

		FTransform HoldingPointTransform = InteractableRootComponent->GetSocketTransform (HoldingPointSocketName);
		FTransform PhysicsHandlePointTransform = InteractableRootComponent->GetSocketTransform (PhysicsHandlePointSocketName);

		LeftHandHoldingActor = Cast<AInteractable> (Actor);
		LeftHandHoldingComponent = ClosestLeftInteractableComponent;
		LeftHandHoldingActor->PickUp (this);

		LeftPhysicsHandle->GrabComponentAtLocationWithRotation (InteractableRootComponent, NAME_None, PhysicsHandlePointTransform.GetLocation (), PhysicsHandlePointTransform.GetRotation ().Rotator ());

		IsLeftHandHolding = true;

		FAttachmentTransformRules AttachmentTransformRules (EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
		LeftHandMesh->AttachToComponent (InteractableRootComponent, AttachmentTransformRules, HoldingPointSocketName);
	}
}

void AVRPlayer::RightGripPressed ()
{
	if (ClosestRightInteractableComponent == nullptr)
	{
		return;
	}

	if (PlayerInteractionType == EInteractionType::RealWorld)
	{
		UPrimitiveComponent* InteractableRootComponent = Cast<UPrimitiveComponent> (ClosestRightInteractableComponent->GetOwner ()->GetRootComponent ());

		FName HoldingPointSocketName;
		FName PhysicsHandlePointSocketName;

		AActor* Actor = ClosestRightInteractableComponent->GetOwner ();
		
		if (Actor->IsA (AWeapon::StaticClass ()))
		{
			AWeapon* WeaponActor = Cast<AWeapon> (Actor);

			if (WeaponActor->GripInteractionZone == ClosestRightInteractableComponent)
			{
				HoldingPointSocketName = FName ("Grip Right Interaction Point");
				PhysicsHandlePointSocketName = FName ("Grip Physics Handle Point");
			}
			else if (WeaponActor->SteadyInteractionZone == ClosestRightInteractableComponent) 
			{
				if (WeaponActor->InteractableType == EInteractableType::SecondaryWeapon) 
				{
					if (WeaponActor->IsHeld) 
					{
						HoldingPointSocketName = FName ("Steady Right Interaction Point");
						PhysicsHandlePointSocketName = FName ("Steady Physics Handle Point");
					}
					else
					{
						ClosestRightInteractableComponent = WeaponActor->GripInteractionZone;
						HoldingPointSocketName = FName ("Grip Right Interaction Point");
						PhysicsHandlePointSocketName = FName ("Grip Physics Handle Point");
					}
				}
				else
				{
					HoldingPointSocketName = FName ("Steady Right Interaction Point");
					PhysicsHandlePointSocketName = FName ("Steady Physics Handle Point");
				}
			}
		}
		else if (Actor->IsA (AGrenadeBase::StaticClass ()))
		{
			AGrenadeBase* GrenadeActor = Cast <AGrenadeBase> (Actor);

			if (GrenadeActor->GripInteractionZone == ClosestRightInteractableComponent)
			{
				HoldingPointSocketName = FName ("Grip Right Interaction Point");
				PhysicsHandlePointSocketName = FName ("Grip Physics Handle Point");
			}
		}
		
		FTransform HoldingPointTransform = InteractableRootComponent->GetSocketTransform (HoldingPointSocketName);
		FTransform PhysicsHandlePointTransform = InteractableRootComponent->GetSocketTransform (PhysicsHandlePointSocketName);

		RightHandHoldingActor = Cast<AInteractable> (Actor);
		RightHandHoldingComponent = ClosestRightInteractableComponent;
		RightHandHoldingActor->PickUp (this);

		RightPhysicsHandle->GrabComponentAtLocationWithRotation (InteractableRootComponent, NAME_None, PhysicsHandlePointTransform.GetLocation (), PhysicsHandlePointTransform.GetRotation ().Rotator ());

		IsRightHandHolding = true;

		FAttachmentTransformRules AttachmentTransformRules (EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
		RightHandMesh->AttachToComponent (InteractableRootComponent, AttachmentTransformRules, HoldingPointSocketName);
	}
}

void AVRPlayer::LeftGripReleased ()
{
	if (PlayerInteractionType == EInteractionType::RealWorld)
	{
		if (IsLeftHandHolding)
		{
			LeftPhysicsHandle->ReleaseComponent ();

			FDetachmentTransformRules DetachmentTransformRules (EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false);
			LeftHandMesh->DetachFromComponent (DetachmentTransformRules);

			FAttachmentTransformRules AttachmentTransformRules (EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
			LeftHandMesh->AttachToComponent (LeftMotionController, AttachmentTransformRules);

			IsLeftHandHolding = false;

			LeftHandHoldingActor->Drop (this);

			LeftHandHoldingActor = nullptr;
			LeftHandHoldingComponent = nullptr;
			ClosestLeftInteractableComponent = nullptr;

			UpdateClosestLeftInteractable ();
		}
	}
}

void AVRPlayer::RightGripReleased ()
{
	if (PlayerInteractionType == EInteractionType::RealWorld)
	{
		if (IsRightHandHolding)
		{
			RightPhysicsHandle->ReleaseComponent ();

			FDetachmentTransformRules DetachmentTransformRules (EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false);
			RightHandMesh->DetachFromComponent (DetachmentTransformRules);

			FAttachmentTransformRules AttachmentTransformRules (EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
			RightHandMesh->AttachToComponent (RightMotionController, AttachmentTransformRules);

			IsRightHandHolding = false;

			RightHandHoldingActor->Drop (this);

			RightHandHoldingActor = nullptr;
			RightHandHoldingComponent = nullptr;
			ClosestRightInteractableComponent = nullptr;

			UpdateClosestRightInteractable ();
		}
	}
}

void AVRPlayer::LeftTriggerPressed ()
{
	if (PlayerInteractionType == EInteractionType::RealWorld)
	{
		if (IsLeftHandHolding && LeftHandHoldingActor->IsA (AWeapon::StaticClass ()) &&	LeftHandHoldingComponent->GetName ().Contains ("Grip")) 
		{
			Cast<AWeapon> (LeftHandHoldingActor)->TriggerPressed ();
		}
	}
	else if (PlayerInteractionType == EInteractionType::UI)
	{
		LeftWidgetInteractionComponent->PressPointerKey (FKey (FName ("LeftMouseButton")));
	}

	if (LeftHandInteractionType == EInteractionType::Grenade)
	{
		// Select the grenade
		LeftHandInteractionType = EInteractionType::RealWorld;
	}
}

void AVRPlayer::LeftTriggerReleased () 
{
	if (PlayerInteractionType == EInteractionType::RealWorld)
	{
		if (IsLeftHandHolding && LeftHandHoldingActor->IsA (AWeapon::StaticClass ()) &&	LeftHandHoldingComponent->GetName ().Contains ("Grip")) 
		{
			Cast<AWeapon> (LeftHandHoldingActor)->TriggerReleased ();
		}
	}
	else if (PlayerInteractionType == EInteractionType::UI)
	{
		LeftWidgetInteractionComponent->ReleasePointerKey (FKey (FName ("LeftMouseButton")));
	}

}

void AVRPlayer::RightTriggerPressed () 
{
	if (PlayerInteractionType == EInteractionType::RealWorld)
	{
		if (IsRightHandHolding && RightHandHoldingActor->IsA (AWeapon::StaticClass ()) && RightHandHoldingComponent->GetName ().Contains ("Grip"))
		{
			Cast<AWeapon> (RightHandHoldingActor)->TriggerPressed ();
		}
	}
	else if (PlayerInteractionType == EInteractionType::UI)
	{
		RightWidgetInteractionComponent->PressPointerKey (FKey (FName ("LeftMouseButton")));
	}
	
	if (RightHandInteractionType == EInteractionType::Grenade)
	{
		// Select the grenade

		RightHandInteractionType = EInteractionType::RealWorld;
	}
}

void AVRPlayer::RightTriggerReleased ()
{
	if (PlayerInteractionType == EInteractionType::RealWorld)
	{
		if (IsRightHandHolding && RightHandHoldingActor->IsA (AWeapon::StaticClass ()) && RightHandHoldingComponent->GetName ().Contains ("Grip")) 
		{
			Cast<AWeapon> (RightHandHoldingActor)->TriggerReleased ();
		}
	}
	else if (PlayerInteractionType == EInteractionType::UI)
	{
		RightWidgetInteractionComponent->ReleasePointerKey (FKey (FName ("LeftMouseButton")));
	}
}

void AVRPlayer::LeftXPressed () 
{
	if (PlayerInteractionType == EInteractionType::RealWorld)
	{
		if (IsLeftHandHolding && LeftHandHoldingActor->IsA (AWeapon::StaticClass ()) && LeftHandHoldingComponent->GetName ().Contains ("Grip")) 
		{
			Cast<AWeapon> (LeftHandHoldingActor)->Reload ();
		}
	}
}

void AVRPlayer::RightAPressed ()
{
	if (PlayerInteractionType == EInteractionType::RealWorld)
	{
		if (IsRightHandHolding && RightHandHoldingActor->IsA (AWeapon::StaticClass ()) && RightHandHoldingComponent->GetName ().Contains ("Grip")) 
		{
			Cast<AWeapon> (RightHandHoldingActor)->Reload ();
		}
	}
}

void AVRPlayer::LeftThumbstickUpDown (float AxisValue)
{
	if (PlayerInteractionType == EInteractionType::RealWorld)
	{
		AddMovementInput (LeftMotionController->GetForwardVector () * AxisValue);
	}	
}

void AVRPlayer::LeftThumbstickLeftRight (float AxisValue)
{
	if (PlayerInteractionType == EInteractionType::RealWorld)
	{
		AddMovementInput (LeftMotionController->GetRightVector () * AxisValue);
	}
}

void AVRPlayer::RightThumbstickUpDown (float AxisValue)
{
	if (PlayerInteractionType == EInteractionType::RealWorld)
	{
		AddMovementInput (RightMotionController->GetForwardVector () * AxisValue);
	}
}

void AVRPlayer::RightThumbstickLeftRight (float AxisValue)
{
	if (PlayerInteractionType == EInteractionType::RealWorld)
	{
		AddMovementInput (RightMotionController->GetRightVector () * AxisValue);
	}	
}

void AVRPlayer::ThumbstickPressed ()
{
	if (PlayerInteractionType == EInteractionType::RealWorld)
	{
		if (!IsFlipped180) 
		{
			AddControllerYawInput (72);
			IsFlipped180 = true;
		} 
		else
		{
			AddControllerYawInput (-72);
			IsFlipped180 = false;
		}
	}
}

void AVRPlayer::LeftThumbstickFull ()
{
	if (PlayerInteractionType == EInteractionType::RealWorld)
	{
		IsLeftThumbStickFull = true;
		UpdateMovementSpeed ();
	}
	
}

void AVRPlayer::LeftThumbstickNotFull ()
{
	if (PlayerInteractionType == EInteractionType::RealWorld)
	{
		IsLeftThumbStickFull = false;
		UpdateMovementSpeed ();
	}
}

void AVRPlayer::RightThumbstickFull ()
{
	if (PlayerInteractionType == EInteractionType::RealWorld)
	{
		IsRightThumbStickFull = true;
		UpdateMovementSpeed ();
	}
}

void AVRPlayer::RightThumbstickNotFull ()
{
	if (PlayerInteractionType == EInteractionType::RealWorld)
	{
		IsRightThumbStickFull = false;
		UpdateMovementSpeed ();
	}
}

void AVRPlayer::UpdateMovementSpeed ()
{
	if (IsRightThumbStickFull && IsLeftThumbStickFull)
	{
		if (GetCharacterMovement ()->IsWalking ())
		{
			GetCharacterMovement ()->MaxWalkSpeed = MaxPlayerRunSpeed;
		}
		
		if (GetCharacterMovement ()->IsCrouching ())
		{
			GetCharacterMovement ()->MaxWalkSpeedCrouched = MaxPlayerCrouchRunSpeed;
		}
	}
	else
	{
		if (GetCharacterMovement ()->IsWalking ()) 
		{
			GetCharacterMovement ()->MaxWalkSpeed = MaxPlayerWalkSpeed;
		} 
		
		if (GetCharacterMovement ()->IsCrouching ())
		{
			GetCharacterMovement ()->MaxWalkSpeedCrouched = MaxPlayerCrouchWalkSpeed;
		}
	}
}

void AVRPlayer::TakeAnyDamage (AActor* DamagedActor, float DamageAmount, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage (++DebugLogCounter, 2.f, FColor::Yellow, TEXT ("On Take Any Damage"));
}

void AVRPlayer::TakePointDamage (AActor* DamagedActor, float DamageAmount, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* HitComponent, FName BoneName, FVector HitDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage (++DebugLogCounter, 2.f, FColor::Yellow, TEXT ("On Take Point Damage"));
}

void AVRPlayer::TakeRadialDamage (AActor* DamagedActor, float DamageAmount, const UDamageType* DamageType, FVector Origin, FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage (++DebugLogCounter, 2.f, FColor::Yellow, TEXT ("On Take Radial Damage"));
}

/*float AVRPlayer::TakeDamage (float DamageAmount, const FDamageEvent& DamageEvent, AController* Instigator, AActor* DamageCauser)
{
	if (Health > 0)
	{
		Health -= Super::TakeDamage (DamageAmount, DamageEvent, Instigator, DamageCauser);
	}

	GEngine->AddOnScreenDebugMessage (++DebugLogCounter, 2.f, FColor::Yellow, TEXT ("VRPlayer Take Damage"));
	GEngine->AddOnScreenDebugMessage (++DebugLogCounter, 2.f, FColor::Yellow, FString::SanitizeFloat (Health));

	CheckHealth ();

	return DamageAmount;
}*/

float AVRPlayer::TakeBulletDamage (float Damage)
{
	Super::TakeBulletDamage (Damage);

	CheckHealth ();

	return 0;
}

float AVRPlayer::TakeExplosiveDamage (float Damage)
{
	Super::TakeExplosiveDamage (Damage);

	Health -= Damage;

	CheckHealth ();

	return 0;
}

float AVRPlayer::TakeFlashDamage (float Damage, FVector FlashLocation)
{
	Super::TakeFlashDamage (Damage, FlashLocation);

	FVector FlashVector = FlashLocation - GetActorLocation ();
	FlashVector.Normalize ();

	float FlashIntensity = FMath::Clamp (FVector::DotProduct (Camera->GetForwardVector (), FlashVector), 0.f, 1.f);
	
	if (FlashIntensity > 0.f) 
	{
		OnFlashed.Broadcast (FlashIntensity);
	}

	return 0;
}

float AVRPlayer::TakeSmokeDamage (float Damage)
{
	Super::TakeSmokeDamage (Damage);

	CheckHealth ();

	return 0;
}

void AVRPlayer::CheckHealth ()
{
	if (Health <= 0) 
	{
		Cast<AVRPlayerController> (GetController ())->SetPlayerPlayState (EPlayerPlayState::Dead);
	}

	Super::CheckHealth ();
}

void AVRPlayer::OnDeathTimerOver ()
{
	Destroy ();
}

void AVRPlayer::ToggleMenu ()
{
	MenuWidget->ToggleVisibility (true);

	if (PlayerInteractionType == EInteractionType::RealWorld)
	{
		PlayerInteractionType = EInteractionType::UI;
	}
	else if (PlayerInteractionType == EInteractionType::UI)
	{
		PlayerInteractionType = EInteractionType::RealWorld;
	}
}

void AVRPlayer::SpawnInteractableOfClass (TSubclassOf <AInteractable> InteractableClass, EInteractableType Type, EInteractableSubType SubType)
{
	FAttachmentTransformRules AttachmentRules (EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	FDetachmentTransformRules DetachmentRules (EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false);

	if (Type == EInteractableType::PrimaryWeapon)
	{
		USceneComponent* AlreadyAttachedInteractable = PrimaryParent->GetChildComponent (0);

		if (AlreadyAttachedInteractable)
		{
			AlreadyAttachedInteractable->GetOwner ()->Destroy ();
		}

		FTransform PrimarySpawnTransform = PrimaryParent->GetComponentTransform ();
		AWeapon* PrimaryActor = Cast<AWeapon> (UGameplayStatics::BeginDeferredActorSpawnFromClass (GetWorld (), InteractableClass, PrimarySpawnTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn, this));
		Cast<UPrimitiveComponent> (PrimaryActor->GetRootComponent ())->SetSimulatePhysics (false);
		PrimaryActor->AttachmentPoint = PrimaryParent;
		UGameplayStatics::FinishSpawningActor (PrimaryActor, PrimarySpawnTransform);
		
		PrimaryActor->AttachToComponent (PrimaryParent, AttachmentRules, NAME_None);
	}
	else if (Type == EInteractableType::SecondaryWeapon)
	{
		USceneComponent* AlreadyAttachedInteractable = SecondaryParent->GetChildComponent (0);

		if (AlreadyAttachedInteractable)
		{
			AlreadyAttachedInteractable->GetOwner ()->Destroy ();
		}

		FTransform SecondarySpawnTransform = SecondaryParent->GetComponentTransform ();
		AWeapon* SecondaryActor = Cast<AWeapon> (UGameplayStatics::BeginDeferredActorSpawnFromClass (GetWorld (), InteractableClass, SecondarySpawnTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn, this));
		Cast<UPrimitiveComponent> (SecondaryActor->GetRootComponent ())->SetSimulatePhysics (false);
		SecondaryActor->AttachmentPoint = SecondaryParent;
		UGameplayStatics::FinishSpawningActor (SecondaryActor, SecondarySpawnTransform);

		SecondaryActor->AttachToComponent (SecondaryParent, AttachmentRules, NAME_None);
	}
	else if (Type == EInteractableType::Grenade)
	{
		if (SubType == EInteractableSubType::FlashGrenade)
		{
			USceneComponent* AlreadyAttachedInteractable = FlashParent->GetChildComponent (0);

			if (AlreadyAttachedInteractable)
			{
				AlreadyAttachedInteractable->GetOwner ()->Destroy ();
			}

			FTransform FlashSpawnTransform = FlashParent->GetComponentTransform ();
			AFlashGrenade* FlashActor = Cast<AFlashGrenade> (UGameplayStatics::BeginDeferredActorSpawnFromClass (GetWorld (), InteractableClass, FlashSpawnTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn, this));
			Cast<UPrimitiveComponent> (FlashActor->GetRootComponent ())->SetSimulatePhysics (false);
			FlashActor->AttachmentPoint = FlashParent;
			UGameplayStatics::FinishSpawningActor (FlashActor, FlashSpawnTransform);

			FlashActor->AttachToComponent (FlashParent, AttachmentRules, NAME_None);
		}
		else if (SubType == EInteractableSubType::ExplosiveGrenade)
		{
			USceneComponent* AlreadyAttachedInteractable = ExplosiveParent->GetChildComponent (0);

			if (AlreadyAttachedInteractable)
			{
				AlreadyAttachedInteractable->GetOwner ()->Destroy ();
			}

			FTransform ExplosiveSpawnTransform = ExplosiveParent->GetComponentTransform ();
			AExplosiveGrenade* ExplosiveActor = Cast<AExplosiveGrenade> (UGameplayStatics::BeginDeferredActorSpawnFromClass (GetWorld (), InteractableClass, ExplosiveSpawnTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn, this));
			Cast<UPrimitiveComponent> (ExplosiveActor->GetRootComponent ())->SetSimulatePhysics (false);
			ExplosiveActor->AttachmentPoint = ExplosiveParent;
			UGameplayStatics::FinishSpawningActor (ExplosiveActor, ExplosiveSpawnTransform);

			ExplosiveActor->AttachToComponent (ExplosiveParent, AttachmentRules, NAME_None);
		}
		else if (SubType == EInteractableSubType::SmokeGrenade)
		{
			USceneComponent* AlreadyAttachedInteractable = SmokeParent->GetChildComponent (0);

			if (AlreadyAttachedInteractable)
			{
				AlreadyAttachedInteractable->GetOwner ()->Destroy ();
			}

			FTransform SmokeSpawnTransform = SmokeParent->GetComponentTransform ();
			ASmokeGrenade* SmokeActor = Cast<ASmokeGrenade> (UGameplayStatics::BeginDeferredActorSpawnFromClass (GetWorld (), InteractableClass, SmokeSpawnTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn, this));
			Cast<UPrimitiveComponent> (SmokeActor->GetRootComponent ())->SetSimulatePhysics (false);
			SmokeActor->AttachmentPoint = SmokeParent;
			UGameplayStatics::FinishSpawningActor (SmokeActor, SmokeSpawnTransform);

			SmokeActor->AttachToComponent (SmokeParent, AttachmentRules, NAME_None);
		}
	}
	else if (Type == EInteractableType::Knife)
	{
	}
}