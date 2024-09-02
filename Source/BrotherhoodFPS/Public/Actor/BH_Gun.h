// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BH_Gun.generated.h"

enum class EPickupType : uint8;

UCLASS()
class BROTHERHOODFPS_API ABH_Gun : public AActor
{
	GENERATED_BODY()

public:
	ABH_Gun();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Gun Components")
	TObjectPtr<USkeletalMeshComponent> Gun;
protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void PlaySoundAndBurstEmitterFX();

	UFUNCTION(BlueprintCallable)
	void ResetCanShoot();

	UFUNCTION(BlueprintCallable)
	AActor* PerformLineTrace();
	
	UFUNCTION(BlueprintCallable)
	void Reload();
	
	UFUNCTION(BlueprintCallable)
	void AddCartridge(int32 NewCartRidge);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Gun Components")
	TObjectPtr<USceneComponent> BurstPoint;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gun FX")
	TObjectPtr<UParticleSystem> ShotBurstFX;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gun FX")
	TObjectPtr<UParticleSystem> ImpactFX;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player Animations")
	TObjectPtr<UAnimMontage> ReloadMontage;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gun Sound")
	TObjectPtr<USoundBase> ShotSound;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gun Sound")
	TObjectPtr<USoundBase> ReloadSound;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gun Params")
	float ShootRate;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gun Params")
	float BulletDamage;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gun Params")
	bool CanShoot;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gun Params")
	bool FireButtonPressed;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gun Params")
	int32 Ammo;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gun Params")
	int32 MaxAmmo;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gun Params")
	int32 Cartridge;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gun Params")
	int32 MaxCartridge;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gun Params")
	EPickupType CartridgeType;
};
