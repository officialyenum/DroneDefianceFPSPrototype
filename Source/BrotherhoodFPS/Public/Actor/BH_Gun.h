// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BH_Gun.generated.h"

class USphereComponent;

USTRUCT(BlueprintType)
struct FGunAttr : public  FTableRowBase
{
	GENERATED_BODY()
public:
	FGunAttr()
		: GunMeshSK(nullptr), ReloadMontage(nullptr), MuzzleFlashVFX(nullptr), ImpactVFX(nullptr), ShotSFX(nullptr),
		  ImpactSFX(nullptr)
	{
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Components",meta=(AllowPrivateAccess = "true"))
	class USkeletalMesh* GunMeshSK;

	UPROPERTY(EditDefaultsOnly, Category="Montage")
	class UAnimMontage* ReloadMontage;
	
	/** Gun Visual and Sound FX */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun FX",meta=(AllowPrivateAccess = "true"))
	UParticleSystem* MuzzleFlashVFX;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun FX",meta=(AllowPrivateAccess = "true"))
	UParticleSystem* ImpactVFX;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun FX",meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> ReloadSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun FX",meta=(AllowPrivateAccess = "true"))
	USoundBase* ShotSFX;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun FX",meta=(AllowPrivateAccess = "true"))
	USoundBase* ImpactSFX;

	/** Gun Parameters */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun Params",meta=(AllowPrivateAccess = "true"))
	FName GunName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun Params",meta=(AllowPrivateAccess = "true"))
	FName MuzzleSocketName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun Params",meta=(AllowPrivateAccess = "true"))
	float Damage = 10.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun Params",meta=(AllowPrivateAccess = "true"))
	float RateOfFire = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun Params",meta=(AllowPrivateAccess = "true"))
	float ReloadTime = 0.4f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun Params",meta=(AllowPrivateAccess = "true"))
	float BulletShotRange = 10000;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun Params",meta=(AllowPrivateAccess = "true"))
	float BulletDamage = 10.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun Params",meta=(AllowPrivateAccess = "true"))
	bool CanShoot = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun Params",meta=(AllowPrivateAccess = "true"))
	bool CanReload = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun Params",meta=(AllowPrivateAccess = "true"))
	int32 Ammo = 30;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun Params",meta=(AllowPrivateAccess = "true"))
	int32 MaxAmmo = 30;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun Params",meta=(AllowPrivateAccess = "true"))
	int32 Cartridge = 4;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun Params",meta=(AllowPrivateAccess = "true"))
	int32 MaxCartridge = 4;
};


UCLASS()
class BROTHERHOODFPS_API ABH_Gun : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USceneComponent> _Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UArrowComponent> _FirePoint;
	
	ABH_Gun();
	
	UFUNCTION(BlueprintCallable)
	virtual void Fire();
	UFUNCTION(BlueprintCallable)
	virtual  void SetGunAttr(const FGunAttr& NewAttr);
	UFUNCTION(BlueprintImplementableEvent, Category = "GlobalEvents")
	void HandleGunFX();
	UFUNCTION(BlueprintImplementableEvent, Category = "GlobalEvents")
	void HandleHitFX(const FVector &ImpactPoint, const FVector &ImpactDirection);
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void SphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FGunAttr GetGunAttr() const { return GunAttr; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UDataTable* GetGunDT() const { return GunDT; }
	FORCEINLINE USphereComponent* GetSphereCollision() const { return SphereCollision; }
	FORCEINLINE USkeletalMeshComponent* GetGunMeshComponent() const { return GunMesh; }
	
	UFUNCTION(BlueprintCallable)
	bool HasAmmo() const;
	UFUNCTION(BlueprintCallable)
	bool HasCartridge() const;
	UFUNCTION(BlueprintCallable)
	void Reload();
	UFUNCTION(BlueprintCallable)
	void AddCartridge(int32 NewCartRidge);
	UFUNCTION(BlueprintCallable)
	void SetCanShoot(bool Value);
	UFUNCTION(BlueprintCallable)
	void SetCanReload(bool Value);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup Components",meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> SphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Components",meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> GunMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun Attr",meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UDataTable> GunDT;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun Attr",meta=(AllowPrivateAccess = "true"))
	FGunAttr GunAttr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Gun Attr",
		meta=(AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
	FName GunName;
	bool GunTrace(FHitResult& OutHit, FVector& ShotDirection) const;
	AController* GetGunOwnerController() const;
	void SetUpGunMesh();
};
