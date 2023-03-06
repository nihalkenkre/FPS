// Fill out your copyright notice in the Description page of Project Settings.

#include "ExplosiveGrenade.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "PhysicsEngine/RadialForceComponent.h"

#include "PlayerBase.h"
#include "AllDamageTypes.h"

AExplosiveGrenade::AExplosiveGrenade ()
{
    /*RadialForce = CreateDefaultSubobject <URadialForceComponent> (FName ("Radial Force"));
    RadialForce->SetupAttachment (GetRootComponent ());

    RadialForce->ImpulseStrength = BaseDamage * 1.5f;
    RadialForce->Radius = OuterRadius;
    RadialForce->Falloff = ERadialImpulseFalloff::RIF_Linear;
    RadialForce->AddObjectTypeToAffect (UEngineTypes::ConvertToObjectType (ECollisionChannel::ECC_GameTraceChannel1));*/
}

void AExplosiveGrenade::PickUp (ACharacter* PlayerCharacter)
{
    Super::PickUp (PlayerCharacter);
}

void AExplosiveGrenade::Drop (ACharacter* PlayerCharacter)
{
    Super::Drop (PlayerCharacter);
    GetWorldTimerManager ().SetTimer (LifetimeTimerHandle, this, &AExplosiveGrenade::OnLifetimeOver, Lifetime, false);
}

void AExplosiveGrenade::OnLifetimeOver ()
{
    TArray <AActor*> ExplosiveHitActors;

    if (UKismetSystemLibrary::SphereOverlapActors (GetWorld (), GetActorLocation (), OuterRadius, TArray <TEnumAsByte <EObjectTypeQuery>> (), APlayerBase::StaticClass (), TArray <AActor*> (), ExplosiveHitActors)) 
    {
        FHitResult HitResult;
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor (this);

        for (int a = 0; a < ExplosiveHitActors.Num (); a++) 
        {
            if (GetWorld ()->LineTraceSingleByChannel (HitResult, GetActorLocation (), ExplosiveHitActors[a]->GetActorLocation (), ECollisionChannel::ECC_Visibility, QueryParams)) 
            {
                if (HitResult.Actor.Get () == ExplosiveHitActors[a]) 
                {
                    APlayerBase* Player = Cast<APlayerBase> (HitResult.Actor.Get ());

                    FVector LocationDifference = Player->GetActorLocation () - GetActorLocation ();
                    FVector DifferenceDirection; float DifferenceMagnitude;

                    LocationDifference.ToDirectionAndLength (DifferenceDirection, DifferenceMagnitude);

                    if (DifferenceMagnitude < InnerRadius) 
                    {
                        //Player->TakeExplosiveDamage (BaseDamage);
						UGameplayStatics::ApplyRadialDamage (GetWorld (), BaseDamage, GetActorLocation (), OuterRadius, UExplosiveDamageType::StaticClass (), TArray<AActor*> (), this, GetInstigatorController ());
                    }
                    else 
                    {
                        if (!DamageCurve) 
                        {
                            //Player->TakeExplosiveDamage (BaseDamage);
							UGameplayStatics::ApplyRadialDamage (GetWorld (), BaseDamage, GetActorLocation (), OuterRadius, UExplosiveDamageType::StaticClass (), TArray<AActor*> (), this, GetInstigatorController ());
                        }
                        else
                        {
                            float SamplePosition = FMath::GetMappedRangeValueClamped (FVector2D (InnerRadius, OuterRadius), FVector2D (0, 1), DifferenceMagnitude - InnerRadius);
                            //Player->TakeExplosiveDamage (BaseDamage * DamageCurve->GetFloatValue (SamplePosition));
							UGameplayStatics::ApplyRadialDamage (GetWorld (), BaseDamage * DamageCurve->GetFloatValue (SamplePosition), GetActorLocation (), OuterRadius, UExplosiveDamageType::StaticClass (), TArray<AActor*> (), this, GetInstigatorController ());
                        }
                    }
                }
            }
        }
    }

    //RadialForce->FireImpulse ();

    Destroy ();
}
