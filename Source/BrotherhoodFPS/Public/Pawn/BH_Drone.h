// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "BH_Drone.generated.h"

class UPawnSensingComponent;

UCLASS()
class BROTHERHOODFPS_API ABH_Drone : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABH_Drone();
	
	UFUNCTION(BlueprintCallable, Category="Drone Functions")
	float GetHealth(){ return Health; };
	UFUNCTION(BlueprintCallable, Category="Drone Functions")
	float GetMaxHealth(){ return MaxHealth; };
	
	UFUNCTION(BlueprintCallable, Category="Drone Functions")
	void SetNextPatrolLocation();
	UFUNCTION(BlueprintCallable, Category="Drone Functions")
	void Chase(APawn* Pawn);

	virtual void PostInitializeComponents() override;

	FORCEINLINE UPawnSensingComponent* GetPawnSense() const { return PawnSense; }
	FORCEINLINE USphereComponent* GetCollision() const { return Coll; }
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPawnDetected(APawn* Pawn);
	UFUNCTION()
	void ExplosionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
	void Explode();
	void ChangeDroneColor(FLinearColor NewColor);
	UFUNCTION()
	void TakeHitDamage(AActor* DamagedActor, float Damage,const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Drone Param")
	TObjectPtr<UParticleSystem> ExplodeFX;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Drone Param")
	TObjectPtr<USoundBase> ExplosionSound;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	// components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone Perception",meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UPawnSensingComponent> PawnSense;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone Perception",meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UFloatingPawnMovement> FloatingPawnMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone Perception",meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> Coll;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone Perception",meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> DroneMesh;
	
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
	float Health =50.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Drone Params",meta=(AllowPrivateAccess = "true"))
	float MaxHealth =0.0f;
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
