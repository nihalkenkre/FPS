#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GrenadeBag.generated.h"

class UBoxComponenet;

UCLASS()
class FPS_API AGrenadeBag : public AInteractable
{
    GENERATED_BODY ()

public:
    int FlashGrenadeCount;
    int ExplosiveGrenadeCount;
    int SmokeGrenadeCount;
};