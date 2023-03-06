// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "AllEnums.h"

#include "CTFFlag.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class FPS_API ACTFFlag : public AActor
{
	GENERATED_BODY ()
	
public:	
	ACTFFlag ();

	UPROPERTY (VisibleAnywhere)
		UStaticMeshComponent* FlagMesh;

	UPROPERTY (VisibleAnywhere)
		UBoxComponent* InteractionZone;

	UPROPERTY (EditAnywhere)
		ECTFFlagType FlagType;
		
	virtual void BeginPlay () override;

protected:
	UFUNCTION ()
		void OnInteractionZoneOverlapBegin (UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool IsFromSweep, const FHitResult& SweepResult);

	UFUNCTION ()
		void OnInteractionZoneOverlapEnd (UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
