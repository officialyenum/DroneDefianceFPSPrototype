// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Character/BH_CharacterSandBox.h"

#include "Component/HealthSystem.h"
#include "Component/WeaponSystem.h"


// Sets default values
ABH_CharacterSandBox::ABH_CharacterSandBox()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	WeaponSystem = CreateDefaultSubobject<UWeaponSystem>("Weapon Manager");
	HealthSystem = CreateDefaultSubobject<UHealthSystem>("Health Manager");
}

// Called when the game starts or when spawned
void ABH_CharacterSandBox::BeginPlay()
{
	Super::BeginPlay();
	if (HealthSystem)
	{
		HealthSystem->OnHealthDamaged.AddDynamic(this, &ABH_CharacterSandBox::HandleHealthDamaged);
		HealthSystem->OnHealthDead.AddDynamic(this, &ABH_CharacterSandBox::HandleHealthDead);
	}
}

void ABH_CharacterSandBox::Fire()
{
	if(WeaponSystem)
	{
		WeaponSystem->FireWeapon();
	}
}

void ABH_CharacterSandBox::HandleHealthDamaged(float NewHealth, float MaxHealth, float HealthChange)
{
	GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("Show UI Damage Was Dealth"));
}

void ABH_CharacterSandBox::HandleHealthLow(float CurrentHealth)
{
}

void ABH_CharacterSandBox::HandleShieldDamaged(float NewShield, float MaxShield, float ShieldChange)
{
}

void ABH_CharacterSandBox::HandleShieldDestroyed(AController* causer)
{
}

void ABH_CharacterSandBox::HandleCartridgeEmpty(FString Message)
{
}

void ABH_CharacterSandBox::HandleReloadStart()
{
}

void ABH_CharacterSandBox::HandleReloadEnd()
{
}

void ABH_CharacterSandBox::HandleHealthDead(AController* causer)
{
	GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("Handle Player Death"));
}

// Called every frame
void ABH_CharacterSandBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

