// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "Animation/BH_AnimInstanceBase.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "BH_CharacterBase.generated.h"

class UWeaponSystem;
class UHealthSystem;
class ABH_Gun;

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	Player UMETA(DisplayName = "Player"),
	Enemy UMETA(DisplayName = "Enemy"),
	NPC UMETA(DisplayName = "NPC"),
};

UCLASS(Abstract)
class BROTHERHOODFPS_API ABH_CharacterBase : public ACharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABH_CharacterBase();
	virtual void Fire() override;
	/**Combat Handlers*/
	UFUNCTION()
	virtual void HandleHealthDamaged(float NewHealth, float NewMaxHealth, float HealthChange) override;
	UFUNCTION()
	virtual void HandleHealthLow(float CurrentHealth) override;
	UFUNCTION()
	virtual void HandleHealthDead(AController* causer) override;
	UFUNCTION()
	virtual void HandleShieldDamaged(float NewShield, float MaxShield, float ShieldChange) override;
	UFUNCTION()
	virtual void HandleShieldDestroyed(AController* causer) override;
	UFUNCTION()
	virtual void HandleCartridgeEmpty(FString Message) override;
	UFUNCTION()
	virtual void HandleReloadStart() override;
	UFUNCTION()
	virtual void HandleReloadEnd() override;
	/**Combat Handlers*/
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdatePlayerParamStats(int32 bullet, int32 cartridge, int32 wasHit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	virtual void ApplyDamageToEnemy(AActor* Actor);
	virtual void TakeHitDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	                           AController* InstigatedBy, AActor* DamageCauser);

	virtual void PostInitializeComponents() override;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void SetUpAnimBp();
	
	UFUNCTION(BlueprintCallable)
	void PlayFireMontage();

	UFUNCTION(BlueprintCallable)
	void FireWeapon();
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player Animations")
	TObjectPtr<UAnimMontage> ReloadMontage;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gun Sound")
	TObjectPtr<USoundBase> ReloadSound;

	UFUNCTION(BlueprintCallable)
	void SetUpWeapon();
	UFUNCTION(BlueprintCallable)
	void SetUpHealth();
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player FX")
	TObjectPtr<UParticleSystem> ShotBurstFX;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player FX")
	TObjectPtr<UParticleSystem> ImpactFX;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player Sound")
	TObjectPtr<USoundBase> ShotSound;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player Animations")
	TObjectPtr<UBH_AnimInstanceBase> AnimBP;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player Animations")
	TObjectPtr<UAnimMontage> AimFireMontage;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player Animations")
	TObjectPtr<UAnimMontage> AimMontage;

	UFUNCTION(BlueprintCallable)
	UWeaponSystem* GetWeaponSystem() const { return WeaponSystem; }
	
	UFUNCTION(BlueprintCallable)
	UHealthSystem* GetHealthSystem() const { return HealthSystem; }

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta=(AllowPrivateAccess= "true"))
	TObjectPtr<class UWeaponSystem> WeaponSystem;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta=(AllowPrivateAccess= "true"))
	TObjectPtr<class UHealthSystem> HealthSystem;
};
