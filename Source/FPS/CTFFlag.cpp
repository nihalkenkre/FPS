// Fill out your copyright notice in the Description page of Project Settings.

#include "CTFFlag.h"

#include "Components/BoxComponent.h"

ACTFFlag::ACTFFlag ()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	FlagMesh = CreateDefaultSubobject<UStaticMeshComponent> (FName ("Flag Mesh"));
	RootComponent = FlagMesh;

	InteractionZone = CreateDefaultSubobject<UBoxComponent> (FName ("Interaction Zone"));
	InteractionZone->SetCollisionProfileName (FName ("Trigger"));
	InteractionZone->SetupAttachment (RootComponent);
}

void ACTFFlag::BeginPlay ()
{
	Super::BeginPlay ();

	InteractionZone->OnComponentBeginOverlap.AddDynamic (this, &ACTFFlag::OnInteractionZoneOverlapBegin);
	InteractionZone->OnComponentEndOverlap.AddDynamic (this, &ACTFFlag::OnInteractionZoneOverlapEnd);
}

void ACTFFlag::OnInteractionZoneOverlapBegin (UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool IsFromSweep, const FHitResult& SweepResult)
{

}

void ACTFFlag::OnInteractionZoneOverlapEnd (UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}