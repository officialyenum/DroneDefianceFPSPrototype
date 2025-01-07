// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "BH_GameMode.h"
#include "BH_CampaignGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BROTHERHOODFPS_API ABH_CampaignGameMode : public ABH_GameMode
{
	GENERATED_BODY()
public:
	ABH_CampaignGameMode();
	virtual void PawnKilled(APawn* PawnKilled) override;
private:
	void EndGame(bool bIsPlayerWinner);
};
