// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Actor/BH_Pickup.h"

#include "Character/BH_CharacterPlayer.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABH_Pickup::ABH_Pickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemMesh");
	ItemMesh->SetupAttachment(RootComponent);
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere");
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetGenerateOverlapEvents(true);
	
	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>("FloatingPawnMovement");
	
	// Default Values
	PickupValue = 20.0f;
	PickupType = EPickupType::Health;
}

// Called when the game starts or when spawned
void ABH_Pickup::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this,&ABH_Pickup::SphereBeginOverlap);
	
}

void ABH_Pickup::SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABH_CharacterPlayer* CHPlayer = Cast<ABH_CharacterPlayer>(OtherActor))
	{
		if (PickupType == EPickupType::Health)
		{
			
			CHPlayer->AddHealth(PickupValue);
		}
		else
		{
			CHPlayer->AddCartridge(PickupValue);
		}
		UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation(), GetActorRotation(), 0.5f);
		Destroy();
	}
}


