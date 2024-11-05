// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Character/BH_Enemy.h"

#include "Character/BH_CharacterPlayer.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
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
	//OnTakeAnyDamage.AddDynamic(this,&ABH_Enemy::TakeHitDamage);
	CharacterType = ECharacterType::Enemy;
	OnTakeAnyDamage.AddDynamic(this,&ABH_Enemy::TakeHitDamage);

	
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
		UGameplayStatics::ApplyDamage(Enemy,BulletDamage,GetController(),this,DamageType);
	}
}

void ABH_Enemy::TakeHitDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
	AActor* DamageCauser)
{
	
	// float NewHealth = Health - Damage;
	// Health = FMath::Clamp(NewHealth, 0, MaxHealth);
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

void ABH_Enemy::SetupStimulusSource()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	if (StimulusSource)
	{
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->RegisterWithPerceptionSystem();
	}
}

