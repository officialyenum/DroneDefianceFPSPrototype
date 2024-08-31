// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Pawn.h"
#include "Interaction/CombatInterface.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "BH_Drone.generated.h"

UCLASS()
class BROTHERHOODFPS_API ABH_Drone : public APawn, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABH_Drone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void ExplosionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void ApplyDamageToEnemy(AActor* Actor) override;

	UFUNCTION()
	virtual void TakeHitDamage(AActor* DamagedActor, float Damage,const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	void Explode();
	UFUNCTION(BlueprintCallable)
	void ChangeDroneColor(bool bIsInRageMode);

	void MoveToGoalTarget(AActor* NewGoalTarget);
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Drone Components")
	TObjectPtr<USphereComponent> ExplosionSphere;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Drone Components")
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Drone Components")
	TObjectPtr<UFloatingPawnMovement> FloatingPawnMovement;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Drone Components")
	TObjectPtr<USkeletalMeshComponent> DroneMesh;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Drone Components")
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Drone Param")
	TObjectPtr<UParticleSystem> ExplodeFX;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Drone Param")
	TObjectPtr<USoundBase> ExplosionSound;
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category="Drone Param")
	TObjectPtr<AActor> GoalActor;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Drone Param")
	double Health;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Drone Param")
	double MaxHealth;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Drone Param")
	double BulletDamage;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
	TObjectPtr<UMaterialInstanceDynamic> DynamicDroneMaterial;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	FLinearColor RageColor;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	FLinearColor CoolColor;
};
