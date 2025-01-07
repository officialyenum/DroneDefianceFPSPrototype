// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Character/BH_Enemy.h"

#include "Actor/BH_Gun.h"
#include "Character/BH_CharacterPlayer.h"
#include "Component/HealthSystem.h"
#include "Component/WeaponSystem.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"


// Sets default values
ABH_Enemy::ABH_Enemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetupStimulusSource();
}

UBehaviorTree* ABH_Enemy::GetBehaviorTree() const
{
	return Tree;
}

// Called when the game starts or when spawned
void ABH_Enemy::BeginPlay()
{
	Super::BeginPlay();
	// CharacterType = ECharacterType::Enemy;
	// OnTakeAnyDamage.AddDynamic(this,&ABH_Enemy::TakeHitDamage);
}

void ABH_Enemy::Fire()
{
	Super::Fire();
	PlayFireMontage();
}

void ABH_Enemy::HandleHealthDamaged(float NewHealth, float NewMaxHealth, float HealthChange)
{
	Super::HandleHealthDamaged(NewHealth, NewMaxHealth, HealthChange);
}

void ABH_Enemy::HandleHealthLow(float CurrentHealth)
{
	Super::HandleHealthLow(CurrentHealth);
}

void ABH_Enemy::HandleHealthDead(AController* causer)
{
	Super::HandleHealthDead(causer);
}

void ABH_Enemy::HandleShieldDamaged(float NewShield, float MaxShield, float ShieldChange)
{
	Super::HandleShieldDamaged(NewShield, MaxShield, ShieldChange);
}

void ABH_Enemy::HandleShieldDestroyed(AController* causer)
{
	Super::HandleShieldDestroyed(causer);
}

void ABH_Enemy::HandleCartridgeEmpty(FString Message)
{
	Super::HandleCartridgeEmpty(Message);
}

void ABH_Enemy::HandleReloadStart()
{
	Super::HandleReloadStart();
}

void ABH_Enemy::HandleReloadEnd()
{
	Super::HandleReloadEnd();
}

void ABH_Enemy::EnterCombatMode()
{
	AnimBP->IsAiming = true;
}

void ABH_Enemy::ExitCombatMode()
{
	AnimBP->IsAiming = false;
}

void ABH_Enemy::ApplyDamageToEnemy(AActor* Actor)
{
	if (ABH_CharacterPlayer* Enemy = Cast<ABH_CharacterPlayer>(Actor))
	{
		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Orange,FString::Printf(TEXT("Damage Applied To Player")));
		TSubclassOf<UDamageType> DamageType;
		UGameplayStatics::ApplyDamage(Enemy,GetWeaponSystem()->GetEquippedGun()->GetGunAttr().BulletDamage,GetController(),this,DamageType);
	}
}

void ABH_Enemy::TakeHitDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
	AActor* DamageCauser)
{
	//float NewHealth = Health - Damage;
	//Health = FMath::Clamp(NewHealth, 0, MaxHealth);
	//GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,FString::Printf(TEXT("Damage Applied To Enemy")));
	//if (Health <= 0)
	//{
		//Play Death Animation
		//Destroy();
	//}
}

// Called every frame
void ABH_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABH_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool ABH_Enemy::IsDead() const
{
	if (GetHealthSystem()->HasHealth())
	{
		return true;
	}
	return false;
}

void ABH_Enemy::SetupStimulusSource()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	if (StimulusSource)
	{
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Hearing>());
		StimulusSource->RegisterWithPerceptionSystem();
	}
}

