// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "Animation/BH_AnimInstanceBase.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Character.h"
#include "BH_CharacterBase.generated.h"

UCLASS(Abstract)
class BROTHERHOODFPS_API ABH_CharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABH_CharacterBase();
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdatePlayerParamStats(int32 bullet, int32 cartridge, int32 wasHit);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void ApplyDamageToEnemy(AActor* Actor);
	virtual void TakeHitDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
							AController* InstigatedBy, AActor* DamageCauser);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void SetUpAnimBp();
	
	UFUNCTION(BlueprintCallable)
	void PlayFireMontage();

	UFUNCTION(BlueprintCallable)
	void PlaySoundAndBurstEmitterFX();

	UFUNCTION(BlueprintCallable)
	void ResetCanShoot();
	
	UFUNCTION(BlueprintCallable)
	void FireWeapon();

	UFUNCTION(BlueprintCallable)
	void PerformLineTrace();
	
	UFUNCTION(BlueprintCallable)
	void ReloadWeapon();

	UFUNCTION(BlueprintCallable)
	void AddCartridge(int32 CartridgeAmount);

	UFUNCTION(BlueprintCallable)
	void AddHealth(float NewHealth);
	
	UFUNCTION(BlueprintCallable)
	void Reload();
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player Animations")
	TObjectPtr<UAnimMontage> ReloadMontage;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gun Sound")
	TObjectPtr<USoundBase> ReloadSound;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Player Components")
	TObjectPtr<USceneComponent> BurstPoint;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Player Components")
	TObjectPtr<USkeletalMeshComponent> Gun;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player FX")
	TObjectPtr<UParticleSystem> ShotBurstFX;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player FX")
	TObjectPtr<UParticleSystem> ImpactFX;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player SoundS")
	TObjectPtr<USoundBase> ShotSound;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player Animations")
	TObjectPtr<UBH_AnimInstanceBase> AnimBP;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player Animations")
	TObjectPtr<UAnimMontage> AimFireMontage;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player Animations")
	TObjectPtr<UAnimMontage> AimMontage;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player Params")
	float Strength;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player Params")
	float MaxStrength;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player Params")
	float Health;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player Params")
	float MaxHealth;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gun Params")
	float ShootRate;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gun Params")
	float BulletDamage;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gun Params")
	bool CanShoot;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gun Params")
	bool FireButtonPressed;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gun Params")
	float ReloadTime;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gun Params")
	int32 Cartridge;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gun Params")
	int32 Ammo;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gun Params")
	int32 MaxCartridge;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gun Params")
	int32 MaxAmmo;
};
