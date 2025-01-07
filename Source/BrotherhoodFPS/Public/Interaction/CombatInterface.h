// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BROTHERHOODFPS_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void HandleHealthDamaged(float NewHealth, float MaxHealth, float HealthChange) = 0;
	virtual void HandleHealthLow(float CurrentHealth) = 0;
	virtual void HandleHealthDead(AController* causer) = 0;

	virtual void HandleShieldDamaged(float NewShield, float MaxShield, float ShieldChange) = 0;
	virtual void HandleShieldDestroyed(AController* causer) = 0;
	
	virtual void HandleCartridgeEmpty(FString Message) = 0;
	virtual void HandleReloadStart() = 0;
	virtual void HandleReloadEnd() = 0;
	virtual void Fire() = 0;
};
