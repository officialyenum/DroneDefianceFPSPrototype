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
	/**Combat Handlers*/
	virtual void HandleHealthDamaged(float NewHealth, float NewMaxHealth, float HealthChange) override;
	virtual void HandleHealthLow(float CurrentHealth) override;
	virtual void HandleHealthDead(AController* causer) override;
	virtual void HandleShieldDamaged(float NewShield, float MaxShield, float ShieldChange) override;
	virtual void HandleShieldDestroyed(AController* causer) override;
	virtual void HandleCartridgeEmpty(FString Message) override;
	virtual void HandleReloadStart() override;
	virtual void HandleReloadEnd() override;
	/**Combat Handlers*/
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
