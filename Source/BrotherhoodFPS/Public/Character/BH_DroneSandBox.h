// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "BH_DroneSandBox.generated.h"

class USphereComponent;
class UAIPerceptionStimuliSourceComponent;
class UBehaviorTree;
class UWeaponSystem;
class UHealthSystem;

UCLASS(Abstract)
class BROTHERHOODFPS_API ABH_DroneSandBox : public ACharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABH_DroneSandBox();

	virtual void PostInitializeComponents() override;
	UFUNCTION(BlueprintCallable)
	void EnterChaseMode();
	UFUNCTION(BlueprintCallable)
	void ExitChaseMode();

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
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Fire() override;
	UFUNCTION()
	void ExplosionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
	void Explode();
	void ChangeDroneColor(FLinearColor NewColor);
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool IsDead() const;
	
	FORCEINLINE UHealthSystem* GetHealthSystem() const { return  HealthSystem; }
	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return  Tree; }
	FORCEINLINE UAIPerceptionStimuliSourceComponent* GetStimulusSource() const { return  StimulusSource; }
	FORCEINLINE USphereComponent* GetSphereCollision() const { return  SphereCollision; }
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Drone Params",meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem> ExplodeFX;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Drone Params",meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> ExplosionSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Drone Params",meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UBehaviorTree> Tree;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Drone Params",meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UAIPerceptionStimuliSourceComponent> StimulusSource;
	
	UFUNCTION()
	void SetupStimulusSource();

	// components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta=(AllowPrivateAccess= "true"))
	TObjectPtr<class UHealthSystem> HealthSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone Perception",meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> SphereCollision;
	
	UPROPERTY()
	FVector StartLocation;
	UPROPERTY()
	FVector PatrolLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Drone Params",meta=(AllowPrivateAccess = "true"))
	float PatrolSpeed = 150.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Drone Params",meta=(AllowPrivateAccess = "true"))
	float ChaseSpeed = 150.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Drone Params",meta=(AllowPrivateAccess = "true"))
	float PatrolRadius = 5000.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Drone Params",meta=(AllowPrivateAccess = "true"))
	float BulletDamage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Drone Params",meta=(AllowPrivateAccess = "true"))
	int PatrolCounter = 2;
	
	UMaterialInstanceDynamic* DynamicDroneMaterial;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Drone Params",meta=(AllowPrivateAccess = "true"))
	FLinearColor RageColor = FLinearColor::Red;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Drone Params",meta=(AllowPrivateAccess = "true"))
	FLinearColor CoolColor= FLinearColor::Blue;
};
