// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "Actor/BH_PatrolPoint.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Pawn.h"
#include "Interaction/CombatInterface.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "BH_Drone.generated.h"

UENUM(BlueprintType)
enum class EDroneState : uint8
{
	Patrol UMETA(DisplayName = "Patrol"),
	Rage UMETA(DisplayName = "Rage")
};

UCLASS()
class BROTHERHOODFPS_API ABH_Drone : public APawn, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABH_Drone();

	UBehaviorTree* GetBehaviorTree() const;
	UFUNCTION(BlueprintCallable, Category="Drone Functions")
	EDroneState GetDroneState(){ return CurrentState; };
	UFUNCTION(BlueprintCallable, Category="Drone Functions")
	ABH_PatrolPoint* GetPatrolPoint(){ return NextPatrolPoint; };
	UFUNCTION(BlueprintCallable, Category="Drone Functions")
	AActor* GetGoalActor(){ return GoalActor; };
	
	
	UFUNCTION(BlueprintCallable, Category="Drone Functions")
	void SetDroneState(EDroneState NewState);
	UFUNCTION(BlueprintCallable, Category="Drone Functions")
	void SetGoalActor(AActor* NewGoal);

	
	ABH_PatrolPoint* GoToNextPatrolPoint();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void ExplosionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void ApplyDamageToEnemy(AActor* Actor) override;
	UFUNCTION(BlueprintCallable)
	void Explode();
	void ChangeDroneColor(FLinearColor NewColor);
	void ResetToPatrol();
	UFUNCTION()
	virtual void TakeHitDamage(AActor* DamagedActor, float Damage,const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser) override;
	UFUNCTION()
	void InitPatrolPoint();
	float GetHealth(){ return Health; };
	float GetMaxHealth(){ return MaxHealth; };
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Drone Param")
	TObjectPtr<UParticleSystem> ExplodeFX;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Drone Param")
	TObjectPtr<USoundBase> ExplosionSound;
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category="Drone Param")
	AActor* GoalActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* Tree;
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category="Drone Param")
	ABH_PatrolPoint* NextPatrolPoint;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	// components
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* DroneMesh;
	
	UPROPERTY(VisibleAnywhere)
	USphereComponent* ExplosionSphere;
	
	UPROPERTY(VisibleAnywhere)
	UFloatingPawnMovement* FloatingPawnMovement;

	UPROPERTY(VisibleAnywhere)
	UAIPerceptionStimuliSourceComponent* StimulusSource;
	void SetupStimulusSource();
	// State
	UPROPERTY(EditDefaultsOnly, Category="Drone Params");
	EDroneState CurrentState;
	UPROPERTY(EditDefaultsOnly, Category="Drone Params");
	float Health;
	UPROPERTY(EditDefaultsOnly, Category="Drone Params");
	float MaxHealth;
	UPROPERTY(EditDefaultsOnly, Category="Drone Params");
	float BulletDamage;
	
	
	UMaterialInstanceDynamic* DynamicDroneMaterial;
	
	UPROPERTY(EditDefaultsOnly, Category="Drone Params")
	FLinearColor RageColor;
	UPROPERTY(EditDefaultsOnly, Category="Drone Params")
	FLinearColor CoolColor;
};
