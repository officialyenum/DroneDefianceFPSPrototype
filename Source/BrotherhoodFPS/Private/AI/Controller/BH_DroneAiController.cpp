// Copyright Chukwuyenum Opone @officialyenum 2024


#include "AI/Controller/BH_DroneAiController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/BH_CharacterPlayer.h"
#include "Character/BH_CharacterPlayerSandBox.h"
#include "Character/BH_DroneSandBox.h"
#include "Kismet/GameplayStatics.h"
#include "Pawn/BH_Drone.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
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
	APawn* PawnPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
	// SetFocus(PawnPlayer);
	
	if (ABH_DroneSandBox* const npc = Cast<ABH_DroneSandBox>(InPawn))
	{
		if (UBehaviorTree* const tree = npc->GetBehaviorTree())
		{
			UBlackboardComponent* b;
			UseBlackboard(tree->BlackboardAsset, b);
			Blackboard = b;
			RunBehaviorTree(tree);
			if (PawnPlayer && npc)
			{
				GetBlackboardComponent()->SetValueAsObject(TEXT("Player"), PawnPlayer);
				GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), npc->GetActorLocation());
			}
		}
	};
}

void ABH_DroneAiController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	if (SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		SightConfig->SightRadius = 4000.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 100.f;
		SightConfig->PeripheralVisionAngleDegrees = 500.f;
		SightConfig->SetMaxAge(5.f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		if(HearingConfig){
			HearingConfig->HearingRange = 3000.0f;
			HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
			HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
			HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
		}
		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ABH_DroneAiController::OnTargetDetected);

		GetPerceptionComponent()->ConfigureSense((*SightConfig));

		// AAIController::SetGenericTeamId(FGenericTeamId(TeamId));
	}
}

void ABH_DroneAiController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (auto* const ch = Cast<ABH_CharacterPlayerSandBox>(Actor)) 
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			Cast<ABH_DroneSandBox>(GetPawn())->EnterChaseMode();
			//TODO: Notify Owner that it has found enemy target
			GetBlackboardComponent()->SetValueAsObject("Player", ch);
			GetBlackboardComponent()->SetValueAsVector("LastKnownLocation", ch->GetActorLocation());
			GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Green,FString::Printf(TEXT("Player Seen")));
		}
		else
		{
			Cast<ABH_DroneSandBox>(GetPawn())->ExitChaseMode();
			//TODO: Notify Owner that it has LOST enemy target but send last known location as well
			GetBlackboardComponent()->ClearValue("Player");
			GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,FString::Printf(TEXT("Player Lost")));
		}
	}
}

void ABH_DroneAiController::BeginPlay()
{
	Super::BeginPlay();
}
