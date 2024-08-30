// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "BH_CharacterBase.h"
#include "BH_NPC.generated.h"

UCLASS()
class BROTHERHOODFPS_API ABH_NPC : public ABH_CharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABH_NPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
