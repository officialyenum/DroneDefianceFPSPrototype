// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Character/BH_Enemy.h"

#include "Character/BH_CharacterPlayer.h"
#include "Engine/DamageEvents.h"


// Sets default values
ABH_Enemy::ABH_Enemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABH_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABH_Enemy::ApplyDamageToEnemy(AActor* Actor)
{
	Super::ApplyDamageToEnemy(Actor);
	if (ABH_CharacterPlayer* Enemy = Cast<ABH_CharacterPlayer>(Actor))
	{
		Enemy->TakeDamage(BulletDamage, DamageType, GetController(), this);
	}
}

float ABH_Enemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                            AActor* DamageCauser)
{
	float NewHealth = Health - DamageAmount;
	Health = FMath::Clamp(NewHealth, 0, MaxHealth);
	return DamageAmount;
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

