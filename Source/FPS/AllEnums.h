#pragma once

#include "AllEnums.generated.h"

UENUM(BlueprintType)
enum class EMatchType : uint8
{
	None UMETA (DisplayName = "None"),
	Deathmatch UMETA (DisplayName = "DeathMatch"),
	CaptureTheFlag UMETA (DisplayName = "Capture The Flag")
};

UENUM(BlueprintType)
enum class EWeaponFireMode : uint8
{
	Single UMETA (DisplayName = "Single"),
	Auto UMETA (DisplayName = "Auto")
};

UENUM(BlueprintType)
enum class EHoldingHand : uint8
{
	None UMETA (DisplayName = "None"),
	Left UMETA (DisplayName = "Left"),
	Right UMETA (DisplayName = "Right")
};

UENUM(BlueprintType)
enum class ETriggerState : uint8
{
	Pressed UMETA (DisplayName = "Pressed"),
	Released UMETA (DisplayName = "Released")
};

UENUM(BlueprintType)
enum class EWeaponShootState : uint8
{
	Ready UMETA (DisplayName = "Ready"),
	NotReady UMETA (DisplayName = "Not Ready")
};

UENUM(BlueprintType)
enum class EInteractableType : uint8
{
	Grenade UMETA (DisplayName = "Grenade"),
	PrimaryWeapon UMETA (DisplayName = "Primary Weapon"),
	SecondaryWeapon UMETA (DisplayName = "Secondary Weapon"),
	Knife UMETA (DisplayName = "Knife")
};

UENUM(BlueprintType)
enum class EInteractableSubType : uint8
{
	FlashGrenade UMETA (DisplayName = "Flash Grenade"),
	SmokeGrenade UMETA (DisplayName = "Smoke Grenade"),
	ExplosiveGrenade UMETA (DisplayName = "Explosive Grenade"),

	Knife UMETA (DisplayName = "Knife"),
	Pistol UMETA (DisplayName = "Pistol"),
	SMG UMETA (DisplayName = "Sub Machine Gun"),
	AssaultRifle UMETA (DisplayName = "Assault Rifle"),
	SniperRifle UMETA (DisplayName = "Sniper Rifle")
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	DesertEagle UMETA (DisplayName = "Desert Eagle"),
	AK47 UMETA (DisplayName = "AK47"),
	AR4 UMETA (DisplayName = "AR4"),
	MAC10 UMETA (DisplayName = "MAC10")
};

UENUM(BlueprintType)
enum class ECTFFlagType : uint8
{
	None UMETA (DisplayName = "None"),
	Red UMETA (DisplayName = "Red"),
	Blue UMETA (DisplayName = "Blue")
};

UENUM(BlueprintType)
enum class ETeamType : uint8
{
	None UMETA (DisplayName = "None"),
	Red UMETA (DisplayName = "Red"),
	Blue UMETA (DisplayName = "Blue")
};

UENUM(BlueprintType)
enum class EPlayerPlayState : uint8
{
	None UMETA (DisplayName = "None"),
	Warmup UMETA (DisplayName = "Warm Up"),
	Playing UMETA (DisplayName = "Playing"),
	Dead UMETA (DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EPlayerType : uint8
{
	None UMETA (DisplayName = "None"),
	VRPlayer UMETA (DisplayName = "VR Player"),
	BOT UMETA (DisplayName = "BOT")
};

UENUM(BlueprintType)
enum class EMatchState : uint8
{
	WaitingToStart UMETA (DisplayName = "Waiting to Start"),
	Started UMETA (DisplayName = "Started"),
	Aborted UMETA (DisplayName = "Aborted"),
	Ended UMETA (DisplayName = "Ended"),
	LeavingMap UMETA (DisplayName = "Leaving Map")
};

UENUM(BlueprintType)
enum class EInteractionType : uint8
{
	RealWorld UMETA (DisplayName = "Real World"),
	UI UMETA (DisplayName = "UI"),
	Grenade UMETA (DisplayName = "Grenade")
};

UENUM(BlueprintType)
enum class EDominantHandType : uint8
{
	Right UMETA (DisplayName = "Right Hand"),
	Left UMETA (DisplayName = "Left Hand")
};