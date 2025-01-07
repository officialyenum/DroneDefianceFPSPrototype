// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "BH_CharacterBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "BH_Enemy.generated.h"

UCLASS()
class BROTHERHOODFPS_API ABH_Enemy : public ABH_CharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABH_Enemy();

	UBehaviorTree* GetBehaviorTree() const;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Fire() override;
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SpawnProjectile();

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
	
	UFUNCTION()
	void EnterCombatMode();
	UFUNCTION()
	void ExitCombatMode();
	
	virtual void ApplyDamageToEnemy(AActor* Actor) override;
	UFUNCTION()
	virtual void TakeHitDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
							AController* InstigatedBy, AActor* DamageCauser) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	bool IsDead() const;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* Tree;
	
	UPROPERTY(VisibleAnywhere)
	UAIPerceptionStimuliSourceComponent* StimulusSource;
	void SetupStimulusSource();
};
