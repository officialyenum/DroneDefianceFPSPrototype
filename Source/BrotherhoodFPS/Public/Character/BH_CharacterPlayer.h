// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "BH_CharacterBase.h"
#include "Camera/CameraComponent.h"
#include "Interaction/CombatInterface.h"
#include "BH_CharacterPlayer.generated.h"

UCLASS()
class BROTHERHOODFPS_API ABH_CharacterPlayer : public ABH_CharacterBase, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABH_CharacterPlayer();

	UFUNCTION(BlueprintCallable, Category="Gun Param")
	int32 GetGunAmmo(){ return Ammo; }
	
	UFUNCTION(BlueprintCallable, Category="Gun Param")
	int32 GetGunCartridge(){ return Cartridge; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player Params")
	FVector RespawnLocation;

	void Die();
	void Respawn();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void ApplyDamageToEnemy(AActor* Actor) override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void CheckPlayerIsDead();
	
	
	UFUNCTION(BlueprintCallable)
	void ApplyCameraShake(TSubclassOf<class UCameraShakeBase> CameraShakeTemplate);
	
	UFUNCTION()
	virtual void TakeHitDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	                        AController* InstigatedBy, AActor* DamageCauser) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void TriggerOutOfAmmoMessageInUI();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="UI")
	TObjectPtr<UUserWidget> HUD;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Player Components")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player Params")
	float DefaultFOV;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player Params")
	bool bHasKey = false;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player Params")
	bool bIsAtEscapeVehicle = false;
};
