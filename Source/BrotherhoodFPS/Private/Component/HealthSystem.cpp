// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Component/HealthSystem.h"

#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UHealthSystem::UHealthSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UHealthSystem::BeginPlay()
{
	Super::BeginPlay();
	InitHealth();
	InitShield();
	InitStrength();
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthSystem::DamageTaken);
	
}

void UHealthSystem::DamageTaken(AActor* damagedActor, float damage, const UDamageType* dmgType, AController* instigator,
	AActor* causer)
{
	if (CharacterAttr.Shield > 0)
	{
		float change = FMath::Min(damage, CharacterAttr.Shield);
		ConsumeShield(change);
		OnShieldDamaged.Broadcast(CharacterAttr.Shield, CharacterAttr.MaxShield, change);
		if(!HasShield())
		{
			OnShieldDestroyed.Broadcast(instigator);
		}
		return;
	}
	float change = FMath::Min(damage, CharacterAttr.Health);
	ConsumeHealth(change);
	// Broadcast Damage
	OnHealthDamaged.Broadcast(CharacterAttr.Health, CharacterAttr.MaxHealth, change);
	if(UKismetMathLibrary::InRange_FloatFloat(CharacterAttr.Health, 1.f, CharacterAttr.MaxHealth * .25f, true, true))
	{
		// Broadcast Low Health
		OnHealthLow.Broadcast(CharacterAttr.Health);
	}
	if(!HasHealth())
	{
		// Broadcast Dead
		OnHealthDead.Broadcast(instigator);
	}
}


// Called every frame
void UHealthSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

void UHealthSystem::InitHealth()
{
	CharacterAttr.Health = GetCharacterAttr().MaxHealth;
}

void UHealthSystem::InitStrength()
{
	CharacterAttr.Strength = CharacterAttr.MaxStrength;
}

void UHealthSystem::InitShield()
{
	CharacterAttr.Shield = CharacterAttr.MaxShield;
}

float UHealthSystem::AddHealth(float HealthToAdd)
{
	CharacterAttr.Health += FMath::Clamp(HealthToAdd, 0.f, CharacterAttr.MaxHealth);
	return CharacterAttr.Health;
}

float UHealthSystem::ConsumeHealth(float HealthToConsume)
{
	CharacterAttr.Health -= FMath::Clamp(HealthToConsume, 0.f, CharacterAttr.MaxHealth);
	return CharacterAttr.Health;
}


float UHealthSystem::AddShield(float ShieldToAdd)
{
	CharacterAttr.Shield += FMath::Clamp(ShieldToAdd, 0.f, CharacterAttr.MaxShield);
	return  CharacterAttr.Shield;
}

float UHealthSystem::ConsumeShield(float ShieldToConsume)
{
	CharacterAttr.Shield -= FMath::Clamp(ShieldToConsume, 0.f, CharacterAttr.MaxShield);
	return  CharacterAttr.Shield;
}

float UHealthSystem::AddStrength(float StrengthToAdd)
{
	CharacterAttr.Strength += FMath::Clamp(StrengthToAdd, 0.f, CharacterAttr.MaxStrength);
	return  CharacterAttr.Strength;
}

float UHealthSystem::ConsumeStrength(float StrengthToConsume)
{
	CharacterAttr.Strength -= FMath::Clamp(StrengthToConsume, 0.f, CharacterAttr.MaxStrength);
	return  CharacterAttr.Strength;
}

float UHealthSystem::GetStrengthPercentage() const
{
	return CharacterAttr.Strength / CharacterAttr.MaxStrength;
}

float UHealthSystem::GetShieldPercentage() const
{
	return CharacterAttr.Shield / CharacterAttr.MaxShield;
}

float UHealthSystem::GetHealthPercentage() const
{
	return CharacterAttr.Health / CharacterAttr.MaxHealth;
}

bool UHealthSystem::HasHealth() const
{
	if (CharacterAttr.Health > 0)
	{
		return true;
	}
	return false;
}

bool UHealthSystem::HasStrength() const
{
	if (CharacterAttr.Strength > 0)
	{
		return true;
	}
	return false;
}

bool UHealthSystem::HasShield() const
{
	if (CharacterAttr.Shield > 0)
	{
		return true;
	}
	return false;
}


