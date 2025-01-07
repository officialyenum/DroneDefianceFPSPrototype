// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Character/BH_Friendly.h"

#include "Character/BH_Enemy.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Pawn/BH_Drone.h"


// Sets default values
ABH_Friendly::ABH_Friendly()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABH_Friendly::BeginPlay()
{
	Super::BeginPlay();
	OnTakeAnyDamage.AddDynamic(this,&ABH_Friendly::TakeHitDamage);
	
}

void ABH_Friendly::Fire()
{
	Super::Fire();
}

void ABH_Friendly::ApplyDamageToEnemy(AActor* Actor)
{
	
}

void ABH_Friendly::TakeHitDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
}

void ABH_Friendly::HandleHealthDamaged(float NewHealth, float NewMaxHealth, float HealthChange)
{
	Super::HandleHealthDamaged(NewHealth, NewMaxHealth, HealthChange);
}

void ABH_Friendly::HandleHealthLow(float CurrentHealth)
{
	Super::HandleHealthLow(CurrentHealth);
}

void ABH_Friendly::HandleHealthDead(AController* causer)
{
	Super::HandleHealthDead(causer);
}

void ABH_Friendly::HandleShieldDamaged(float NewShield, float MaxShield, float ShieldChange)
{
	Super::HandleShieldDamaged(NewShield, MaxShield, ShieldChange);
}

void ABH_Friendly::HandleShieldDestroyed(AController* causer)
{
	Super::HandleShieldDestroyed(causer);
}

void ABH_Friendly::HandleCartridgeEmpty(FString Message)
{
	Super::HandleCartridgeEmpty(Message);
}

void ABH_Friendly::HandleReloadStart()
{
	Super::HandleReloadStart();
}

void ABH_Friendly::HandleReloadEnd()
{
	Super::HandleReloadEnd();
}

// Called every frame
void ABH_Friendly::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABH_Friendly::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

