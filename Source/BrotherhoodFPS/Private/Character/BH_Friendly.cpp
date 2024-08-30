// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Character/BH_Friendly.h"

#include "Character/BH_Enemy.h"
#include "Engine/DamageEvents.h"


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
	
}

void ABH_Friendly::ApplyDamageToEnemy(AActor* Actor)
{
	Super::ApplyDamageToEnemy(Actor);
	if (ABH_Enemy* Enemy = Cast<ABH_Enemy>(Actor))
	{
		Enemy->TakeDamage(BulletDamage, DamageType, GetController(), this);
	}
}

float ABH_Friendly::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float NewHealth = Health - DamageAmount;
	Health = FMath::Clamp(NewHealth, 0, MaxHealth);
	return DamageAmount;
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

