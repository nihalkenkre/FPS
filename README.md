# First Person Shooter

Work in progress source code for a virtual reality based first person shooter, with features similar to Counter Strike.


## Gameplay Class Diagram.

```mermaid
classDiagram
    Interactable <|-- GrenadeBase
    Interactable <|-- Weapon
    PlayerBase <|-- VRPlayer
    PlayerBase <|-- BOT
    AActor <|-- Bullet

    class Interactable {
        Mesh
        InteractionZone
        InteractableType
        InteractableSubType
        Pickup()
        Drop()
    }

    note for GrenadeBase "Smoke, Flash, Explosive derive from this and implement the damage in OnLifetimeOver"

    class GrenadeBase {
        BaseDamage
        MinimumDamage
        OuterRadius
        InnerRadius
        DamageFalloff
        DamageCurve
        PickUp()
        Drop()
    }

    class Weapon {
        SteadyInteractionZone
        Magazine
        MaximumBullets
        RoundsPerMinute
        RecoilCurve
        BulletSpreadCurve
        SingleShotAudio
        ReloadStartAudio
        EmptyShotAudio
        BulletClass
        BulletTrailClass
        BulletTrailSpeed
        OnTriggerPressed()
        OnTriggerReleased()
        Reload()
        PickUp()
        Drop()
        Shoot()
        FireBullet()
        FireBulletHitScan()
        AddRecoil
        GenerateBulletTrail ()
    }
    
    class Bullet {
        Mesh
        TrailMesh
        Lifetime
        OnHit()
        OnLifetimeOver()
    }

    class PlayerBase {
        Health
        CameraComponent

        DelegatesForDamage()
    }

    note for VRPlayer "All plural properties stand for Right and Left hands"

    class VRPlayer {
        MotionControllers
        WidgetInteractionComponents
        HandMeshes
        InteractionZones
        PhysicsHandles
        PrimaryWeaponParent
        SecondaryWeaponParent

        ControllerButtonsAndTriggerResponseFunnctions()

        DeleagatesForInteractions()
        DelegatesForGranadeDamage()
        CheckHealth()
    }

    class BOT {
        MaxWalkSpeed
        MaxRunSpeed

        DelegatesForGrenadeDamage()
        CheckHealth()
    }

```

## Points
* Weapons recoil differently when held with one and hand or two, with two hands providing more stability than one hand.
* The recoil of the gun is physics based and driven by a recoil curve. This curve is in effect when firing in full automatic mode i.e. recoil amount generally increases as more shots are fired before the 'cool down' period is over, which is the case in full automatic mode.
* The recoil is implemented using the Physics Handle component and 'cool down' period is the time it takes for the physics handle to reach the 'initial position'.
* Bullet spread is controlled by the bullet spread curve, which generally increases as more bullets are shot in full automatic mode.