// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BH_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class BROTHERHOODFPS_API ABH_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void PawnKilled(APawn* PawnKilled);
};
