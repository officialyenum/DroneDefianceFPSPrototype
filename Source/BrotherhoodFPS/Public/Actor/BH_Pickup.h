// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "BH_Pickup.generated.h"

class ABH_Gun;

UENUM(BlueprintType)
enum class EPickupType : uint8
{
	Health UMETA(DisplayName = "Health"),
	Cartridge UMETA(DisplayName = "Rifle_Cartridge"),
	Pistol_Cartridge UMETA(DisplayName = "Pistol_Cartridge"),
	Gun UMETA(DisplayName = "Gun"),
};

UCLASS()
class BROTHERHOODFPS_API ABH_Pickup : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABH_Pickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void SphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Pickup", meta=(DisplayName="Pickup"))
	void PickUp(class ABH_CharacterSandBox* OwningCharacter);
public:
	FORCEINLINE USphereComponent* GetSphereCollision() const { return SphereCollision; }
	FORCEINLINE UStaticMeshComponent* GetPickUpMesh() const { return PickUpMesh; }
	
	
	UFUNCTION()
	void DestroyPickup();
	
private:
	// components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup Components",meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> SphereCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup Components",meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> PickUpMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup Components",meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class URotatingMovementComponent> RotatingMovementComponent;
	// params
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup Params",meta=(AllowPrivateAccess = "true"))
	float PickupValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup Params",meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> PickupSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup Params",meta=(AllowPrivateAccess = "true"))
	EPickupType PickupType;
};
