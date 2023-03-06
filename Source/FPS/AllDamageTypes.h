#pragma once

#include "GameFramework/DamageType.h"
#include "AllDamageTypes.generated.h"

UCLASS()
class FPS_API UBulletDamageType : public UDamageType
{
	GENERATED_BODY ()
};

UCLASS()
class FPS_API UFlashDamageType : public UDamageType
{
    GENERATED_BODY ()
};

UCLASS()
class FPS_API UExplosiveDamageType : public UDamageType
{
    GENERATED_BODY ()
};

UCLASS()
class FPS_API USmokeDamageType : public UDamageType
{
    GENERATED_BODY ()
};

UCLASS()
class FPS_API UFireDamageType : public UDamageType
{
    GENERATED_BODY ()
};