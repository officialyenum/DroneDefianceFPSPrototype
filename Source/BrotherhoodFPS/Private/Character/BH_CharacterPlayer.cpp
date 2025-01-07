// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Character/BH_CharacterPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Actor/BH_Gun.h"
#include "Character/BH_Enemy.h"
#include "Component/HealthSystem.h"
#include "Component/WeaponSystem.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
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
	Camera->SetFieldOfView(90.0f);

	RespawnLocation = FVector(-1970.000000,6280.000000,98.000100);
}

FGunAttr ABH_CharacterPlayer::GetGunAttr() const
{
	if(GetWeaponSystem())
	{
		return GetWeaponSystem()->GetEquippedGun()->GetGunAttr();
	}
	return FGunAttr();
}

void ABH_CharacterPlayer::HandleHealthDamaged(float NewHealth, float NewMaxHealth, float HealthChange)
{
	Super::HandleHealthDamaged(NewHealth, NewMaxHealth, HealthChange);
}

void ABH_CharacterPlayer::HandleHealthLow(float CurrentHealth)
{
	Super::HandleHealthLow(CurrentHealth);
}

void ABH_CharacterPlayer::HandleHealthDead(AController* causer)
{
	Super::HandleHealthDead(causer);
}

void ABH_CharacterPlayer::HandleShieldDamaged(float NewShield, float MaxShield, float ShieldChange)
{
	Super::HandleShieldDamaged(NewShield, MaxShield, ShieldChange);
}

void ABH_CharacterPlayer::HandleShieldDestroyed(AController* causer)
{
	Super::HandleShieldDestroyed(causer);
}

void ABH_CharacterPlayer::HandleCartridgeEmpty(FString Message)
{
	Super::HandleCartridgeEmpty(Message);
}

void ABH_CharacterPlayer::HandleReloadStart()
{
	Super::HandleReloadStart();
}

void ABH_CharacterPlayer::HandleReloadEnd()
{
	Super::HandleReloadEnd();
}

void ABH_CharacterPlayer::Die()
{
	Respawn();
}

void ABH_CharacterPlayer::Respawn()
{
	if (GetHealthSystem())
	{
		GetHealthSystem()->InitHealth();
	}
	SetActorLocation(RespawnLocation);
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
	TSubclassOf<UDamageType> DamageType;
	float damage = GetWeaponSystem()->GetEquippedGun()->GetGunAttr().BulletDamage;
	if (ABH_Enemy* Enemy = Cast<ABH_Enemy>(Actor))
	{
		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Black,FString::Printf(TEXT("Damage Applied To Enemy")));
		UGameplayStatics::ApplyDamage(Enemy, damage,GetController(),this,DamageType);
	}
	if (AActor* Enemy = Cast<AActor>(Actor))
	{
		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Green,FString::Printf(TEXT("Damage Applied To Drone")));
		UGameplayStatics::ApplyDamage(Enemy, damage, GetController(),this,DamageType);
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
	GetHealthSystem()->DamageTaken(DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);
	GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,FString::Printf(TEXT("Player Took Damage")));
	UpdatePlayerParamStats(0, 0, Damage);
	if (!GetHealthSystem()->HasHealth())
	{
		Die();
	}
}

// Called every frame
void ABH_CharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetHealthSystem()->GetCharacterAttr().Strength < GetHealthSystem()->GetCharacterAttr().MaxStrength && AnimBP->IsSprinting == false)
	{
		GetHealthSystem()->AddStrength(5.0f * DeltaTime);
	}
	if (GetHealthSystem()->GetCharacterAttr().Strength > 0 && AnimBP->IsSprinting == true)
	{
		GetHealthSystem()->ConsumeStrength(20.0f * DeltaTime);
		if(GetHealthSystem()->GetCharacterAttr().Strength <= 0)
		{
			GetCharacterMovement()->MaxWalkSpeed = 600.0f;
			AnimBP->IsSprinting = false;
		}
	}
}

void ABH_CharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

