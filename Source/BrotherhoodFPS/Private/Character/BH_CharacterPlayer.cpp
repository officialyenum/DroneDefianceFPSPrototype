// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Character/BH_CharacterPlayer.h"

#include "Character/BH_Enemy.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Pawn/BH_Drone.h"
#include "Player/BH_PlayerController.h"


// Sets default values
ABH_CharacterPlayer::ABH_CharacterPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(GetCapsuleComponent());
}

// Called when the game starts or when spawned
void ABH_CharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	OnTakeAnyDamage.AddDynamic(this,&ABH_CharacterPlayer::TakeHitDamage);
	SetUpAnimBp();
	DefaultFOV = Camera->FieldOfView; // Store default FOV
}

void ABH_CharacterPlayer::ApplyDamageToEnemy(AActor* Actor)
{
	Super::ApplyDamageToEnemy(Actor);
	TSubclassOf<UDamageType> DamageType;
	if (ABH_Enemy* Enemy = Cast<ABH_Enemy>(Actor))
	{
		UGameplayStatics::ApplyDamage(Enemy,BulletDamage,GetController(),this,DamageType);
	}
	if (AActor* Enemy = Cast<AActor>(Actor))
	{
		UGameplayStatics::ApplyDamage(Enemy, BulletDamage,GetController(),this,DamageType);
	}
}

void ABH_CharacterPlayer::ApplyCameraShake(TSubclassOf<class UCameraShakeBase> CameraShakeTemplate)
{
	if(CameraShakeTemplate)
	{
		GetController<ABH_PlayerController>()->ClientStartCameraShake(CameraShakeTemplate);
	}
}

void ABH_CharacterPlayer::TakeHitDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                        AController* InstigatedBy, AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,FString::Printf(TEXT("Player Took Damage")));
		
	float NewHealth = Health - Damage;
	Health = FMath::Clamp(NewHealth, 0, MaxHealth);
	CheckPlayerIsDead();
	UpdatePlayerParamStats(0, 0, Damage);
}

// Called every frame
void ABH_CharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Strength < MaxStrength && AnimBP->IsSprinting == false)
	{
		Strength += 5.0f * DeltaTime;
	}
	if (Strength > 0 && AnimBP->IsSprinting == true)
	{
		Strength -= 20.0f * DeltaTime;
		if(Strength <= 0)
		{
			GetCharacterMovement()->MaxWalkSpeed = 600.0f;
			AnimBP->IsSprinting = false;
		}
	}
}

