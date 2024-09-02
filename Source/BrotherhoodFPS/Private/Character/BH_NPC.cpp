// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Character/BH_NPC.h"


// Sets default values
ABH_NPC::ABH_NPC()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABH_NPC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABH_NPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABH_NPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

