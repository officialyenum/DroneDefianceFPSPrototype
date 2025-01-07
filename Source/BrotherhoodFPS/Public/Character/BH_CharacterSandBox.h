// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "BH_CharacterSandBox.generated.h"

class UWeaponSystem;
class UHealthSystem;

UCLASS(Abstract)
class BROTHERHOODFPS_API ABH_CharacterSandBox : public ACharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABH_CharacterSandBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
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
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	FORCEINLINE UWeaponSystem* GetWeaponSystem() const { return  WeaponSystem; }
	FORCEINLINE UHealthSystem* GetHealthSystem() const { return  HealthSystem; }
private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta=(AllowPrivateAccess= "true"))
	TObjectPtr<class UWeaponSystem> WeaponSystem;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta=(AllowPrivateAccess= "true"))
	TObjectPtr<class UHealthSystem> HealthSystem;

};
