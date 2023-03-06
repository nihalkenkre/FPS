// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable.h"
#include "Components/BoxComponent.h"

#include "VRPlayer.h"

AInteractable::AInteractable ()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	InteractableMesh = CreateDefaultSubobject<USkeletalMeshComponent> (FName ("Interactable Mesh"));
	RootComponent = InteractableMesh;

	GripInteractionZone = CreateDefaultSubobject<UBoxComponent> (FName ("Grip Interaction Zone"));
	GripInteractionZone->SetCollisionProfileName (FName ("Trigger"));
	GripInteractionZone->SetupAttachment (GetRootComponent ());

	InteractableMesh->SetCollisionProfileName ("Interactable");
	InteractableMesh->SetSimulatePhysics (true);

	ThresholdDistance = 20.f;
}

void AInteractable::PickUp (ACharacter* PlayerCharacter) 
{
	IsHeld = true;

	FDetachmentTransformRules DetachmentRules (EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false);
	InteractableMesh->DetachFromComponent (DetachmentRules);

	InteractableMesh->SetSimulatePhysics (true);
}

void AInteractable::Drop (ACharacter* PlayerCharacter)
{
	IsHeld = false;

	if (AttachmentPoint && FVector::Distance (AttachmentPoint->GetComponentLocation (), InteractableMesh->GetComponentLocation ()) < ThresholdDistance)
	{
		USceneComponent* AlreadyAttachedComponent = AttachmentPoint->GetChildComponent (0);
		
		if (AlreadyAttachedComponent) 
		{
			FDetachmentTransformRules DetachmentRules (EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false);
			AlreadyAttachedComponent->DetachFromComponent (DetachmentRules);

			UPrimitiveComponent* AlreadyAttachedComponentPrimitive = Cast<UPrimitiveComponent> (AlreadyAttachedComponent);
			AlreadyAttachedComponentPrimitive->SetSimulatePhysics (true);
			AlreadyAttachedComponentPrimitive->SetPhysicsLinearVelocity (AlreadyAttachedComponent->GetRightVector () * 100);
			AlreadyAttachedComponentPrimitive->SetPhysicsAngularVelocityInDegrees (AlreadyAttachedComponent->GetRightVector () * 100);
		}

		InteractableMesh->SetSimulatePhysics (false);

		FAttachmentTransformRules AttachmentRules (EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
		InteractableMesh->AttachToComponent (AttachmentPoint, AttachmentRules, NAME_None);
	}

	AttachmentPoint = nullptr;
}