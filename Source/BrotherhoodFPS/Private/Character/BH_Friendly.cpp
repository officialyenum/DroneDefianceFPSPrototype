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

void ABH_Friendly::ApplyDamageToEnemy(AActor* Actor)
{
	TSubclassOf<UDamageType> DamageType;
	if (ABH_Enemy* Enemy = Cast<ABH_Enemy>(Actor))
	{
		UGameplayStatics::ApplyDamage(Enemy, BulletDamage,GetController(),this,DamageType);
	}
	if (ABH_Drone* Enemy = Cast<ABH_Drone>(Actor))
	{
		UGameplayStatics::ApplyDamage(Enemy, BulletDamage,GetController(),this,DamageType);
	}
}

void ABH_Friendly::TakeHitDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	
	float NewHealth = Health - Damage;
	Health = FMath::Clamp(NewHealth, 0, MaxHealth);
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

