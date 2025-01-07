// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Character/BH_CharacterBase.h"

#include "Character/Animation/BH_AnimInstanceBase.h"
#include "Component/HealthSystem.h"
#include "Component/WeaponSystem.h"
#include "Components/CapsuleComponent.h"


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
	HealthSystem = CreateDefaultSubobject<UHealthSystem>("Health Manager");
	WeaponSystem = CreateDefaultSubobject<UWeaponSystem>("Weapon Manager");
}

void ABH_CharacterBase::Fire()
{
	if(WeaponSystem)
		PlayFireMontage();
		WeaponSystem->FireWeapon();
}

void ABH_CharacterBase::HandleHealthDamaged(float NewHealth, float NewMaxHealth, float HealthChange)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f,FColor::Black,TEXT("Health Damaged"));
}

void ABH_CharacterBase::HandleHealthLow(float CurrentHealth)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f,FColor::Black,TEXT("Health Low"));
}

void ABH_CharacterBase::HandleHealthDead(AController* causer)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f,FColor::Black,TEXT("You Died"));
}

void ABH_CharacterBase::HandleShieldDamaged(float NewShield, float MaxShield, float ShieldChange)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 
			3.0f,
			FColor::Black, 
			FString::Printf(TEXT("Shield Damaged By: %f"), ShieldChange) // Use FString::Printf for formatting
		);
	}
}

void ABH_CharacterBase::HandleShieldDestroyed(AController* causer)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f,FColor::Black,TEXT("Shield Destroyed"));
}

void ABH_CharacterBase::HandleCartridgeEmpty(FString Message)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 
			3.0f, 
			FColor::Black, 
			FString::Printf(TEXT("Message: %s"), *Message) // Use FString::Printf for formatting
		);
	}
}

void ABH_CharacterBase::HandleReloadStart()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f,FColor::Black,TEXT("Reload Start"));
}

void ABH_CharacterBase::HandleReloadEnd()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f,FColor::Black,TEXT("Reload End"));
}

// Called when the game starts or when spawned
void ABH_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	SetUpAnimBp();
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

void ABH_CharacterBase::FireWeapon()
{
	if(WeaponSystem)
		WeaponSystem->FireWeapon();
}

void ABH_CharacterBase::SetUpWeapon()
{
	if (WeaponSystem)
	{
		WeaponSystem->OnCartridgeEmpty.AddDynamic(this,  &ABH_CharacterBase::HandleCartridgeEmpty);
		WeaponSystem->OnWeaponReloadStarted.AddDynamic(this,  &ABH_CharacterBase::HandleReloadStart);
		WeaponSystem->OnWeaponReloadEnded.AddDynamic(this,  &ABH_CharacterBase::HandleReloadEnd);
	}
}

void ABH_CharacterBase::SetUpHealth()
{
	if (HealthSystem)
	{
		HealthSystem->OnHealthDamaged.AddDynamic(this, &ABH_CharacterBase::HandleHealthDamaged);
		HealthSystem->OnHealthLow.AddDynamic(this, &ABH_CharacterBase::HandleHealthLow);
		HealthSystem->OnHealthDead.AddDynamic(this, &ABH_CharacterBase::HandleHealthDead);

		HealthSystem->OnShieldDamaged.AddDynamic(this, &ABH_CharacterBase::HandleShieldDamaged);
		HealthSystem->OnShieldDestroyed.AddDynamic(this, &ABH_CharacterBase::HandleShieldDestroyed);
	}
}

void ABH_CharacterBase::ApplyDamageToEnemy(AActor* Actor)
{
}

void ABH_CharacterBase::TakeHitDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	
}

void ABH_CharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SetUpWeapon();
	SetUpHealth();
}
