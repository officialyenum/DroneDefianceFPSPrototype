// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Character/BH_NPC.h"


// Sets default values
ABH_NPC::ABH_NPC()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ABH_NPC::HandleHealthDamaged(float NewHealth, float NewMaxHealth, float HealthChange)
{
	Super::HandleHealthDamaged(NewHealth, NewMaxHealth, HealthChange);
}

void ABH_NPC::HandleHealthLow(float CurrentHealth)
{
	Super::HandleHealthLow(CurrentHealth);
}

void ABH_NPC::HandleHealthDead(AController* causer)
{
	Super::HandleHealthDead(causer);
}

void ABH_NPC::HandleShieldDamaged(float NewShield, float MaxShield, float ShieldChange)
{
	Super::HandleShieldDamaged(NewShield, MaxShield, ShieldChange);
}

void ABH_NPC::HandleShieldDestroyed(AController* causer)
{
	Super::HandleShieldDestroyed(causer);
}

void ABH_NPC::HandleCartridgeEmpty(FString Message)
{
	Super::HandleCartridgeEmpty(Message);
}

void ABH_NPC::HandleReloadStart()
{
	Super::HandleReloadStart();
}

void ABH_NPC::HandleReloadEnd()
{
	Super::HandleReloadEnd();
}

// Called when the game starts or when spawned
void ABH_NPC::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABH_NPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABH_NPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

