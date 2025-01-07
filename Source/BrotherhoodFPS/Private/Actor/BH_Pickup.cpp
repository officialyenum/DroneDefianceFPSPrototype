// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Actor/BH_Pickup.h"

#include "Actor/BH_Gun.h"
#include "Character/BH_CharacterPlayer.h"
#include "Character/BH_CharacterPlayerSandBox.h"
#include "Character/BH_CharacterSandBox.h"
#include "Component/HealthSystem.h"
#include "Component/WeaponSystem.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABH_Pickup::ABH_Pickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	SetRootComponent(RootComponent);
	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	GetSphereCollision()->SetupAttachment(RootComponent);
	GetSphereCollision()->SetGenerateOverlapEvents(true);
	GetSphereCollision()->SetSphereRadius(200.f);
	
	PickUpMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickUpMesh");
	GetPickUpMesh()->SetupAttachment(GetSphereCollision());
	GetPickUpMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>("FloatingPawnMovement");
	
	// Default Values
	PickupValue = 20.0f;
	PickupType = EPickupType::Health;
}

// Called when the game starts or when spawned
void ABH_Pickup::BeginPlay()
{
	Super::BeginPlay();
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this,&ABH_Pickup::SphereBeginOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &ABH_Pickup::SphereEndOverlap);
}

void ABH_Pickup::DestroyPickup()
{
	UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation(), GetActorRotation(), 0.5f);
	Destroy();
}

void ABH_Pickup::SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABH_CharacterPlayer* CHPlayer = Cast<ABH_CharacterPlayer>(OtherActor))
	{
		switch (PickupType)
		{
			case EPickupType::Health:
				CHPlayer->GetHealthSystem()->AddHealth(PickupValue);
				CHPlayer->UpdateBloodAnimation();
				DestroyPickup();
				break;
			case EPickupType::Cartridge:
				CHPlayer->GetWeaponSystem()->GetEquippedGun()->AddCartridge(PickupValue);
				DestroyPickup();
				break;
			default: ;
		}
	}
	if(ABH_CharacterPlayerSandBox* SandBoxPlayer = Cast<ABH_CharacterPlayerSandBox>(OtherActor))
	{
		PickUp(SandBoxPlayer);
	}
}

void ABH_Pickup::SphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ABH_CharacterPlayer* CHPlayer = Cast<ABH_CharacterPlayer>(OtherActor))
	{
		SetOwner(nullptr);
	}
}

void ABH_Pickup::PickUp_Implementation(ABH_CharacterSandBox* OwningCharacter)
{
	SetOwner(OwningCharacter);
}


