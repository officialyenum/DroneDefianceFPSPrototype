// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Component/WeaponSystem.h"

#include "Actor/BH_Gun.h"
#include "Character/BH_CharacterBase.h"


// Sets default values for this component's properties
UWeaponSystem::UWeaponSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void UWeaponSystem::BeginPlay()
{
	Super::BeginPlay();
	if (GunClass)
	{
		// Spawn the gun
		EquippedGun = GetWorld()->SpawnActor<ABH_Gun>(GunClass);
		if (EquippedGun)
		{
			// Ensure the owner is valid and castable
			ABH_CharacterBase* OwnerCharacter = Cast<ABH_CharacterBase>(GetOwner());
			if (OwnerCharacter && OwnerCharacter->GetMesh())
			{
				
				FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
				// Attach the gun to the character's mesh at "GripPoint"
				EquippedGun->GetGunMeshComponent()->AttachToComponent(
					OwnerCharacter->GetMesh(),
					Rules,
					TEXT("GripPoint")
				);
				EquippedGun->GetGunMeshComponent()->SetVisibility(true);
				EquippedGun->SetOwner(OwnerCharacter);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to attach gun. Owner or Mesh is null."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn gun of class: %s"), *GunClass->GetName());
		}
	}
}


void UWeaponSystem::UpdateHoveredGun(ABH_Gun* NewHoveredGun)
{
	HoveredGun = NewHoveredGun;
}

void UWeaponSystem::FireWeapon()
{
	if (EquippedGun)
	{
		if(!EquippedGun->HasAmmo())
		{
			if(EquippedGun->GetGunAttr().CanReload)
			{
				OnCartridgeEmpty.Broadcast(TEXT("Out Of Ammo, Reloading..."));
				ReloadWeapon();
				return;
			}
			OnCartridgeEmpty.Broadcast(TEXT("Out Of Ammo, Reload"));
			return;
		}
		if(!EquippedGun->GetGunAttr().CanShoot) return;
		EquippedGun->Fire();
		GetWorld()->GetTimerManager().SetTimer(TimerFireDelay, this, &UWeaponSystem::Handle_FireDelay, GetEquippedGun()->GetGunAttr().RateOfFire, false);
	}
}

void UWeaponSystem::ReloadWeapon()
{
	if (EquippedGun->HasCartridge())
	{
		OnWeaponReloadStarted.Broadcast();
		EquippedGun->Reload();
		bIsReloading = true;
		GetWorld()->GetTimerManager().SetTimer(TimerFireDelay, this, &UWeaponSystem::Handle_FireDelay, GetEquippedGun()->GetGunAttr().ReloadTime, false);
		return;
	}
	OnCartridgeEmpty.Broadcast(TEXT("Out Of Cartridge, Find Some To Get Back In The Fight"));
}

void UWeaponSystem::PickUpNewWeapon() const
{
	if (HoveredGun == nullptr) return;
	if (EquippedGun == nullptr)
	{
		const auto Parent = Cast<ABH_CharacterBase>(GetOwner());
		FGunAttr TempAttr = EquippedGun->GetGunAttr();
		EquippedGun->SetGunAttr(HoveredGun->GetGunAttr());
		HoveredGun->SetGunAttr(TempAttr);
		EquippedGun->SetOwner(Parent);
		HoveredGun->SetOwner(nullptr);
	}
}


// Called every frame
void UWeaponSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWeaponSystem::Handle_FireDelay()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerFireDelay);
	if(bIsReloading)
	{
		bIsReloading = false;
		OnWeaponReloadEnded.Broadcast();
	}
	GetEquippedGun()->SetCanShoot(true);
}

