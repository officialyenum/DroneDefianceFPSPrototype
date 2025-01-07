// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponSystem.generated.h"


class ABH_Gun;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponProjectileFiredSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponCartridgeEmptySignature,const FString, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponReloadStartedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponReloadEndedSignature);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BROTHERHOODFPS_API UWeaponSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWeaponSystem();
	
	UPROPERTY(BlueprintAssignable)
	FWeaponProjectileFiredSignature OnProjectileFired;
	UPROPERTY(BlueprintAssignable)
	FWeaponCartridgeEmptySignature OnCartridgeEmpty;
	UPROPERTY(BlueprintAssignable)
	FWeaponReloadStartedSignature OnWeaponReloadStarted;
	UPROPERTY(BlueprintAssignable)
	FWeaponReloadEndedSignature OnWeaponReloadEnded;
	void FireWeapon();
	void ReloadWeapon();
	void UpdateHoveredGun(ABH_Gun* NewHoveredGun);
	void PickUpNewWeapon() const;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, Category="Weapon Component|Time")
	FTimerHandle TimerFireDelay;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ABH_Gun* GetEquippedGun() const { return  EquippedGun; }
	FORCEINLINE ABH_Gun* GetHoveredGun() const { return  HoveredGun; }
	
	UFUNCTION()
	void Handle_FireDelay();

private:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Component|Gun",meta=(AllowPrivateAccess = "true"))
	TSubclassOf<class ABH_Gun> GunClass;

	UPROPERTY()
	bool bIsReloading = false;
	UPROPERTY()
	TObjectPtr<class ABH_Gun> EquippedGun;
	UPROPERTY()
	TObjectPtr<class ABH_Gun> HoveredGun;
};
