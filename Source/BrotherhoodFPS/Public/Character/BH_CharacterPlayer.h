// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "BH_CharacterBase.h"
#include "Camera/CameraComponent.h"
#include "BH_CharacterPlayer.generated.h"

UCLASS()
class BROTHERHOODFPS_API ABH_CharacterPlayer : public ABH_CharacterBase
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	ABH_CharacterPlayer();

	UFUNCTION(BlueprintPure, Category="Gun Param")
	FGunAttr GetGunAttr() const;

	/**Combat Handlers*/
	virtual void HandleHealthDamaged(float NewHealth, float NewMaxHealth, float HealthChange) override;
	virtual void HandleHealthLow(float CurrentHealth) override;
	virtual void HandleHealthDead(AController* causer) override;
	virtual void HandleShieldDamaged(float NewShield, float MaxShield, float ShieldChange) override;
	virtual void HandleShieldDestroyed(AController* causer) override;
	virtual void HandleCartridgeEmpty(FString Message) override;
	virtual void HandleReloadStart() override;
	virtual void HandleReloadEnd() override;
	/**Combat Handlers*/
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player Params")
	FVector RespawnLocation;

	void Die();
	void Respawn();

	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void ApplyDamageToEnemy(AActor* Actor) override;
	
	UFUNCTION(BlueprintCallable)
	void ApplyCameraShake(TSubclassOf<class UCameraShakeBase> CameraShakeTemplate);
	
	UFUNCTION()
	virtual void TakeHitDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	                        AController* InstigatedBy, AActor* DamageCauser) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateBloodAnimation();
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player Params")
	float DefaultFOV;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player Params")
	bool bHasKey = false;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player Params")
	bool bIsAtEscapeVehicle = false;
	UPROPERTY()
	class ABH_Pickup* PickupRef;
protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|UI",meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UUserWidget> HUD;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Camera",meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;
};
