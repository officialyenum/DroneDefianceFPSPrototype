// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Character/BH_CharacterBase.h"

#include "Character/Animation/BH_AnimInstanceBase.h"
#include "Components/CapsuleComponent.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABH_CharacterBase::ABH_CharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Overlap);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(false);
	
	PrimaryGun = CreateDefaultSubobject<UChildActorComponent>("PrimaryGun");
	PrimaryGun->SetupAttachment(GetMesh());
	SecondaryGun = CreateDefaultSubobject<UChildActorComponent>("SecondaryGun");
	SecondaryGun->SetupAttachment(GetMesh());
	//FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
	//Gun->AttachToComponent(GetMesh(), Rules, FName("GripPoint"));
}

// Called when the game starts or when spawned
void ABH_CharacterBase::BeginPlay()
{
	Super::BeginPlay();

	GunInventory.Add(PrimaryGun);
	GunInventory.Add(SecondaryGun);
	EquippedGunIndex = 0;
	EquipWeapon();
}

// Called every frame
void ABH_CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABH_CharacterBase::SetUpAnimBp()
{
	AnimBP = Cast<UBH_AnimInstanceBase>(GetMesh()->GetAnimInstance());
}

void ABH_CharacterBase::PlayFireMontage()
{
	AnimBP->IsAiming ? AnimBP->Montage_Play(AimFireMontage): AnimBP->Montage_Play(AimMontage);
}

void ABH_CharacterBase::ResetCanShoot()
{
	EquippedGun->CanShoot = true;
}

void ABH_CharacterBase::FireWeapon()
{
	if (!EquippedGun->CanShoot || EquippedGun->Ammo <= 0)
	{
		return;
	}
	EquippedGun->CanShoot = false;
	EquippedGun->PlaySoundAndBurstEmitterFX();
	PlayFireMontage();
	PerformLineTrace();
	EquippedGun->Ammo--;
	// delay for 2 seconds
	// Create a timer handle
	FTimerHandle TimerHandle;

	// Bind the ResetCanShoot function to be called after 2 seconds
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABH_CharacterBase::ResetCanShoot, EquippedGun->ShootRate, false);
}

void ABH_CharacterBase::EquipWeapon()
{
	EquippedGunIndex = FMath::Clamp(EquippedGunIndex, 0, GunInventory.Num());
	if (EquippedGunIndex > GunInventory.Num() - 1)
	{
		EquippedGunIndex = 0;
	}
	for (auto Element : GunInventory	)
	{
		Element->SetVisibility(false);
	}
	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
	GunInventory[EquippedGunIndex]->AttachToComponent(GetMesh(), Rules, FName("GripPoint"));
	GunInventory[EquippedGunIndex]->SetVisibility(true);
	EquippedGun = Cast<ABH_Gun>(GunInventory[EquippedGunIndex]->GetChildActor());
}

void ABH_CharacterBase::PerformLineTrace()
{
	AActor* Camera = UGameplayStatics::GetPlayerCameraManager(this,0);
	FVector Loc = Camera->GetActorLocation();
	FVector FV = Camera->GetActorForwardVector() * 5000;
	FVector LOCFV = Loc + FV;

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Loc, LOCFV, ECC_Visibility);
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactFX, HitResult.ImpactPoint,FRotator(0.F),FVector(1));
	if (ICombatInterface* Actor = Cast<ICombatInterface>(HitResult.GetActor()))
	{
		ApplyDamageToEnemy(HitResult.GetActor());
	}
}

void ABH_CharacterBase::ReloadWeapon()
{
	EquippedGun->Reload();
	AnimBP->Montage_Play(EquippedGun->ReloadMontage);
}

void ABH_CharacterBase::ApplyDamageToEnemy(AActor* Actor)
{
}

void ABH_CharacterBase::TakeHitDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
}

void ABH_CharacterBase::AddHealth(float NewHealth)
{
	Health += FMath::Clamp(NewHealth, 0, MaxHealth);
}

void ABH_CharacterBase::AddCartridge(EPickupType CartridgeType, float CartridgeAmount)
{
	for (auto Element : GunInventory)
	{
		if (ABH_Gun* CurrentGun = Cast<ABH_Gun>(Element->GetChildActor()))
		{
			if(CurrentGun->CartridgeType == CartridgeType)
			{
				CurrentGun->AddCartridge(CartridgeAmount);
			}
		}
		
	}
}

