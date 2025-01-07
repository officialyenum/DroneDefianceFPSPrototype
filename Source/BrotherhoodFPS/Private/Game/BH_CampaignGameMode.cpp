// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Game/BH_CampaignGameMode.h"

#include "EngineUtils.h"
#include "AI/Controller/BH_EnemyAiController.h"
#include "Game/BH_GameState.h"
#include "Player/BH_PlayerController.h"

ABH_CampaignGameMode::ABH_CampaignGameMode()
{
	GameStateClass = ABH_GameState::StaticClass();
}

void ABH_CampaignGameMode::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);

	UE_LOG(LogTemp, Display, TEXT("Pawn Killed : %s"),*PawnKilled->GetName());
	APlayerController* PC = Cast<APlayerController>(PawnKilled->GetController());
	if(PC != nullptr)
	{
		EndGame(false);
	}

	for (ABH_EnemyAiController* Controller : TActorRange<ABH_EnemyAiController>(GetWorld()))
	{
		if (!Controller->IsDead())
		{
			return;
		}
	}
	EndGame(true);
}

void ABH_CampaignGameMode::EndGame(bool bIsPlayerWinner)
{
}
