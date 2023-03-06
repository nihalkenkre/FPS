// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuPawn.h"

#include "MotionControllerComponent.h"
#include "HeadMountedDisplay/Public/HeadMountedDisplayFunctionLibrary.h"

#include "Camera/CameraComponent.h"

#include "Components/BoxComponent.h"
#include "Components/LineBatchComponent.h"
#include "Components/WidgetInteractionComponent.h"

#include "Kismet/KismetMathLibrary.h"

AMenuPawn::AMenuPawn ()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoReceiveInput = EAutoReceiveInput::Player0;

	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = nullptr;

	VRScene = CreateDefaultSubobject<USceneComponent> (FName ("VR Scene"));
	VRScene->SetupAttachment (GetRootComponent ());

	Camera = CreateDefaultSubobject<UCameraComponent> (FName ("VR Camera"));
	Camera->SetupAttachment (VRScene);

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
}

void AMenuPawn::BeginPlay ()
{
	Super::BeginPlay ();

	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin (EHMDTrackingOrigin::Floor);
}

void AMenuPawn::SetupPlayerInputComponent (UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent (PlayerInputComponent);

	PlayerInputComponent->BindAction ("Left Trigger", IE_Pressed, this, &AMenuPawn::LeftTriggerPressed);
	PlayerInputComponent->BindAction ("Left Trigger", IE_Released, this, &AMenuPawn::LeftTriggerReleased);

	PlayerInputComponent->BindAction ("Right Trigger", IE_Pressed, this, &AMenuPawn::RightTriggerPressed);
	PlayerInputComponent->BindAction ("Right Trigger", IE_Released, this, &AMenuPawn::RightTriggerReleased);
}

void AMenuPawn::Tick (float DeltaTime)
{
	Super::Tick (DeltaTime);

	FVector LeftEndLocation = LeftMotionController->GetComponentLocation () + (LeftMotionController->GetForwardVector () * LeftWidgetInteractionComponent->InteractionDistance);

	FHitResult HitResult;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor (this);

	if (GetWorld ()->LineTraceSingleByChannel (HitResult, LeftMotionController->GetComponentLocation (), LeftEndLocation, ECollisionChannel::ECC_Visibility, QueryParams))
	{
		GetWorld ()->LineBatcher->DrawLine (LeftMotionController->GetComponentLocation (), HitResult.ImpactPoint, FLinearColor::White, 0, 1.f);
	}
	else
	{
		GetWorld ()->LineBatcher->DrawLine (LeftMotionController->GetComponentLocation (), LeftEndLocation, FLinearColor::White, 0, 1.f);
	}

	FVector RightEndLocation = RightMotionController->GetComponentLocation () + (RightMotionController->GetForwardVector () * RightWidgetInteractionComponent->InteractionDistance);

	if (GetWorld ()->LineTraceSingleByChannel (HitResult, RightMotionController->GetComponentLocation (), RightEndLocation, ECollisionChannel::ECC_Visibility, QueryParams)) 
	{
		GetWorld ()->LineBatcher->DrawLine (RightMotionController->GetComponentLocation (), HitResult.ImpactPoint, FLinearColor::White, 0, 1.f);
	}
	else
	{
		GetWorld ()->LineBatcher->DrawLine (RightMotionController->GetComponentLocation (), RightEndLocation, FLinearColor::White, 0, 1.f);
	}
}

void AMenuPawn::LeftTriggerPressed ()
{
	LeftWidgetInteractionComponent->PressPointerKey (FKey (FName ("LeftMouseButton")));
}

void AMenuPawn::LeftTriggerReleased ()
{
	LeftWidgetInteractionComponent->ReleasePointerKey (FKey (FName ("LeftMouseButton")));
}

void AMenuPawn::RightTriggerPressed ()
{
	RightWidgetInteractionComponent->PressPointerKey (FKey (FName ("LeftMouseButton")));
}

void AMenuPawn::RightTriggerReleased ()
{
	RightWidgetInteractionComponent->ReleasePointerKey (FKey (FName ("LeftMouseButton")));
}