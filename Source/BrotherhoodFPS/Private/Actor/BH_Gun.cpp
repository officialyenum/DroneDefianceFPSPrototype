// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Actor/BH_Gun.h"

#include "CollisionDebugDrawingPublic.h"
#include "Character/BH_CharacterBase.h"
#include "Character/BH_CharacterPlayer.h"
#include "Character/BH_CharacterPlayerSandBox.h"
#include "Character/BH_CharacterSandBox.h"
#include "Component/WeaponSystem.h"
#include "Components/ArrowComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABH_Gun::ABH_Gun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	_Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = _Root;
	
	_FirePoint = CreateDefaultSubobject<UArrowComponent>(TEXT("FirePoint"));
	_FirePoint->SetupAttachment(RootComponent);
	
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Mesh"));
	GunMesh->SetupAttachment(RootComponent);
	GunMesh->SetVisibility(true);
}

void ABH_Gun::Fire()
{
	GunAttr.CanShoot = false;
	GunAttr.Ammo--;
	// MuzzleFlashSocket
	UE_LOG(LogTemp, Display, TEXT("You've Been Shot"));
	if(GunAttr.MuzzleFlashVFX && GunAttr.ShotSFX)
	{
		UE_LOG(LogTemp, Display, TEXT("Muzzle Found"));
		// HandleGunFX();
		// UGameplayStatics::SpawnEmitterAttached(GunAttr.MuzzleFlashVFX, GunMesh, GunAttr.MuzzleSocketName);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GunAttr.ImpactVFX, _FirePoint->GetComponentLocation());
		UGameplayStatics::SpawnSoundAttached(GunAttr.ShotSFX, GunMesh, GunAttr.MuzzleSocketName);
		
		FVector ShotDirection;
		FHitResult OutHitResult;
		bool bHitDetected = GunTrace(OutHitResult, ShotDirection);
		UE_LOG(LogTemp, Display, TEXT("Hit Detected: false"));
		if (bHitDetected)
		{
			UE_LOG(LogTemp, Display, TEXT("Hit Target"));
			if (GunAttr.ImpactVFX && GunAttr.ImpactSFX)
			{
				// HandleHitFX(OutHitResult.ImpactPoint, ShotDirection);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GunAttr.ImpactVFX, OutHitResult.ImpactPoint, ShotDirection.Rotation());
				UGameplayStatics::PlaySoundAtLocation(GetWorld(),GunAttr.ImpactSFX,OutHitResult.ImpactPoint,ShotDirection.Rotation());
				if(AActor* HitActor = OutHitResult.GetActor())
				{
					FPointDamageEvent DamageEvent(GunAttr.Damage, OutHitResult, ShotDirection, nullptr);
					HitActor->TakeDamage(GunAttr.Damage, DamageEvent, GetGunOwnerController(), this);				
				}
			}
			else
			{
				DrawDebugSphere(GetWorld(),OutHitResult.ImpactPoint,100, 16,FColor::Red);
			}
		}
		// DrawDebugCamera(GetWorld(), Location, Rotator, 90, 2, FColor::Red, true);
	}
}

void ABH_Gun::SetGunAttr(const FGunAttr& NewAttr)
{
	GunAttr = NewAttr;
	GunName = GunAttr.GunName;
	SetUpGunMesh();
}

// Called when the game starts or when spawned
void ABH_Gun::BeginPlay()
{
	Super::BeginPlay();
	SetUpGunMesh();
	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
	// Attach the gun to the character's mesh at "GripPoint"
	_FirePoint->AttachToComponent(
		GunMesh,
		Rules,
		GunAttr.MuzzleSocketName
	);
}

bool ABH_Gun::HasAmmo() const
{
	return GunAttr.Ammo > 0;
}

bool ABH_Gun::HasCartridge() const
{
	return GunAttr.Cartridge > 0;
}

void ABH_Gun::SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(ABH_CharacterPlayer* SandBoxPlayer = Cast<ABH_CharacterPlayer>(OtherActor))
	{
		SetOwner(SandBoxPlayer);
		SandBoxPlayer->GetWeaponSystem()->UpdateHoveredGun(this);
	}
}

void ABH_Gun::SphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ABH_CharacterPlayer* CHPlayer = Cast<ABH_CharacterPlayer>(OtherActor))
	{
		SetOwner(nullptr);
		CHPlayer->GetWeaponSystem()->UpdateHoveredGun(nullptr);
	}
}

void ABH_Gun::Reload()
{
	if (GunAttr.Cartridge > 0)
	{
		GunAttr.Cartridge--;
		GunAttr.Ammo = FMath::Clamp(GunAttr.MaxAmmo, 0, GunAttr.MaxAmmo);
		UGameplayStatics::PlaySoundAtLocation(this, GunAttr.ReloadSound, GetActorLocation(), GetActorRotation(), 0.5f);
	}
}

void ABH_Gun::AddCartridge(int32 NewCartRidge)
{
	GunAttr.Cartridge = FMath::Clamp(GunAttr.Cartridge + NewCartRidge, 0, GunAttr.MaxCartridge);
}

void ABH_Gun::SetCanShoot(bool Value)
{
	GunAttr.CanShoot = Value;
}

void ABH_Gun::SetCanReload(bool Value)
{
	GunAttr.CanReload = Value;
}

bool ABH_Gun::GunTrace(FHitResult& OutHit, FVector& ShotDirection) const
{
	AController* OwnerController = GetGunOwnerController();
	if (OwnerController == nullptr) return false;
	FVector Location;
	FRotator Rotator;
	OwnerController->GetPlayerViewPoint(Location, Rotator);
	ShotDirection = -Rotator.Vector();
	FVector EndPoint = Location + Rotator.Vector() * GunAttr.BulletShotRange;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	return GetWorld()->LineTraceSingleByChannel(OutHit, Location, EndPoint, ECC_Visibility, Params);
}

AController* ABH_Gun::GetGunOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return nullptr;
	return OwnerPawn->GetController();
}

void ABH_Gun::SetUpGunMesh()
{
	if (GetGunDT())
	{
		// Find the row for the gun by GunName
		if (const FGunAttr* FoundGunAttr = GetGunDT()->FindRow<FGunAttr>(GunName, TEXT("Lookup Gun Attributes")))
		{
			GunAttr = *FoundGunAttr;

			// Update the skeletal mesh if it's valid
			if (GunAttr.GunMeshSK)
			{
				GunMesh->SetSkeletalMesh(GunAttr.GunMeshSK);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("GunMeshSK is null for GunName: %s"), *GunName.ToString());
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to find row in Gun Data Table for GunName: %s"), *GunName.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Gun Data Table is null."));
	}

}



