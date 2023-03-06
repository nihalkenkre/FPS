// Fill out your copyright notice in the Description page of Project Settings.

#include "FlashGrenade.h"

#include "Camera/CameraComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "PlayerBase.h"
#include "AllDamageTypes.h"

void AFlashGrenade::PickUp (ACharacter* PlayerCharacter)
{
    Super::PickUp (PlayerCharacter);
}

void AFlashGrenade::Drop (ACharacter* PlayerCharacter)
{
    Super::Drop (PlayerCharacter);
    GetWorldTimerManager ().SetTimer (LifetimeTimerHandle, this, &AFlashGrenade::OnLifetimeOver, Lifetime, false);
}

void AFlashGrenade::OnLifetimeOver ()
{
    TArray <AActor*> FlashHitActors;

    if (UKismetSystemLibrary::SphereOverlapActors (GetWorld (), GetActorLocation (), OuterRadius, TArray <TEnumAsByte <EObjectTypeQuery>> (), APlayerBase::StaticClass (), TArray <AActor*> (), FlashHitActors)) 
    {
        FHitResult HitResult;
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor (this);

        for (int a = 0; a < FlashHitActors.Num (); a++) 
        {
            FVector EndTrace = Cast<APlayerBase> (FlashHitActors[a])->Camera->GetComponentLocation ();

            if (GetWorld ()->LineTraceSingleByChannel (HitResult, GetActorLocation (), EndTrace, ECollisionChannel::ECC_Visibility, QueryParams)) 
            {
                if (HitResult.Actor.Get () == FlashHitActors[a]) 
                {
                    APlayerBase* Player = Cast<APlayerBase> (HitResult.Actor.Get ());

                    FVector LocationDifference = Player->GetActorLocation () - GetActorLocation ();
                    FVector DifferenceDirection; float DifferenceMagnitude;

                    LocationDifference.ToDirectionAndLength (DifferenceDirection, DifferenceMagnitude);

                    if (DifferenceMagnitude < InnerRadius) 
                    {
                        //Player->TakeFlashDamage (BaseDamage, GetActorLocation ());
                        UGameplayStatics::ApplyRadialDamage (GetWorld (), BaseDamage, GetActorLocation (), OuterRadius, UFlashDamageType::StaticClass (), TArray<AActor*> (), this, GetInstigatorController ());
                    }
                    else
                    {
                        if (!DamageCurve)
                        {
                            //Player->TakeFlashDamage (BaseDamage, GetActorLocation ());
                            UGameplayStatics::ApplyRadialDamage (GetWorld (), BaseDamage, GetActorLocation (), OuterRadius, UFlashDamageType::StaticClass (), TArray<AActor*> (), this, GetInstigatorController ());
                        }
                        else
                        {
                            float SamplePosition = FMath::GetMappedRangeValueClamped (FVector2D (InnerRadius, OuterRadius), FVector2D (0, 1), DifferenceMagnitude - InnerRadius);
                            //Player->TakeFlashDamage (BaseDamage * DamageCurve->GetFloatValue (SamplePosition), GetActorLocation ());
                            UGameplayStatics::ApplyRadialDamage (GetWorld (), BaseDamage * DamageCurve->GetFloatValue (SamplePosition), GetActorLocation (), OuterRadius, UFlashDamageType::StaticClass (), TArray<AActor*> (), this, GetInstigatorController ());
                        }
                    }
                }
            }
        }
    }

    Destroy ();
}