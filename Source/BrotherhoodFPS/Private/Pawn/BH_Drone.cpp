// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Pawn/BH_Drone.h"


// Sets default values
ABH_Drone::ABH_Drone()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABH_Drone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABH_Drone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABH_Drone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

