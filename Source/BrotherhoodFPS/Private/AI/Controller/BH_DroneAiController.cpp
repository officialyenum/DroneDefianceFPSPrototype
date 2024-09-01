// Copyright Chukwuyenum Opone @officialyenum 2024


#include "AI/Controller/BH_DroneAiController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/BH_CharacterPlayer.h"
#include "Pawn/BH_Drone.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


ABH_DroneAiController::ABH_DroneAiController(FObjectInitializer const& ObjectInitializer)
{
	SetupPerceptionSystem();
}

ETeamAttitude::Type ABH_DroneAiController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if(APawn const* OtherPawn = Cast<APawn>(&Other))
	{
		if (auto const TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			if(TeamAgent->GetGenericTeamId() == FGenericTeamId(0) || TeamAgent->GetGenericTeamId() == FGenericTeamId(1))
			{
				return ETeamAttitude::Friendly;
			}
			else
			{
				return ETeamAttitude::Hostile;
			}
		}
	}
	return ETeamAttitude::Neutral;
}

void ABH_DroneAiController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (ABH_Drone* const npc = Cast<ABH_Drone>(InPawn))
	{
		if (UBehaviorTree* const tree = npc->GetBehaviorTree())
		{
			UBlackboardComponent* b;
			UseBlackboard(tree->BlackboardAsset, b);
			Blackboard = b;
			RunBehaviorTree(tree);
		}
	};
}

void ABH_DroneAiController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		SightConfig->SightRadius = 1000.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 100.f;
		SightConfig->PeripheralVisionAngleDegrees = 500.f;
		SightConfig->SetMaxAge(5.f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ABH_DroneAiController::OnTargetDetected);

		GetPerceptionComponent()->ConfigureSense((*SightConfig));

		// AAIController::SetGenericTeamId(FGenericTeamId(TeamId));
	}
}

void ABH_DroneAiController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (auto* const ch = Cast<ABH_CharacterPlayer>(Actor)) 
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			Cast<ABH_Drone>(GetPawn())->SetGoalActor(Actor);
			Cast<ABH_Drone>(GetPawn())->SetDroneState(EDroneState::Rage);
			GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Green,FString::Printf(TEXT("Player Seen")));
		
		}
		else
		{
			Cast<ABH_Drone>(GetPawn())->SetGoalActor(nullptr);
			Cast<ABH_Drone>(GetPawn())->SetDroneState(EDroneState::Patrol);
			GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,FString::Printf(TEXT("Player Lost")));
		}
		GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
	}
}

void ABH_DroneAiController::OnDroneHit(AActor* Actor)
{
	GetBlackboardComponent()->SetValueAsObject("GoalPointTarget", Actor);
}
