// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "BH_Pickup.generated.h"

UENUM(BlueprintType)
enum class EPickupType : uint8
{
	Health UMETA(DisplayName = "Health"),
	Rifle_Cartridge UMETA(DisplayName = "Rifle_Cartridge"),
	Pistol_Cartridge UMETA(DisplayName = "Pistol_Cartridge"),
	Shotgun_Cartridge UMETA(DisplayName = "Shotgun_Cartridge")
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

public:

	UPROPERTY(VisibleAnywhere, Category="Pickup Components")
	UStaticMeshComponent* ItemMesh;
	
	UPROPERTY(VisibleAnywhere, Category="Pickup Components")
	USphereComponent* SphereComponent;
	
	UPROPERTY(VisibleAnywhere)
	URotatingMovementComponent* RotatingMovementComponent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Pickup Params")
	float PickupValue;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player SoundS")
	TObjectPtr<USoundBase> PickupSound;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Pickup Params")
	EPickupType PickupType;
	
};
