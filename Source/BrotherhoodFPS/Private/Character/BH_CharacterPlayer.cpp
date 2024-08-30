// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Character/BH_CharacterPlayer.h"

#include "Character/BH_Enemy.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Damage.h"


// Sets default values
ABH_CharacterPlayer::ABH_CharacterPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABH_CharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABH_CharacterPlayer::ApplyDamageToEnemy(AActor* Actor)
{
	Super::ApplyDamageToEnemy(Actor);
	if (ABH_Enemy* Enemy = Cast<ABH_Enemy>(Actor))
	{
		Enemy->TakeDamage(BulletDamage, DamageType, GetController(), this);
	}
}

float ABH_CharacterPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float NewHealth = Health - DamageAmount;
	Health = FMath::Clamp(NewHealth, 0, MaxHealth);
	return DamageAmount;
}

// Called every frame
void ABH_CharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABH_CharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

