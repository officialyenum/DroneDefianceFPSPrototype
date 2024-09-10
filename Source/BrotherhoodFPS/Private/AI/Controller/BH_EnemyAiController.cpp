// Copyright Chukwuyenum Opone @officialyenum 2024


#include "AI/Controller/BH_EnemyAiController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/BH_CharacterPlayer.h"
#include "Perception/AIPerceptionTypes.h"

#include "Character/BH_Enemy.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


ABH_EnemyAiController::ABH_EnemyAiController(FObjectInitializer const& ObjectInitializer)
{
	SetupPerceptionSystem();
}

ETeamAttitude::Type ABH_EnemyAiController::GetTeamAttitudeTowards(const AActor& Other) const
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

void ABH_EnemyAiController::OnEnemyHit(AActor* Actor)
{
	// Set blackboard values
	GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", true);
	GetBlackboardComponent()->SetValueAsObject("PlayerTarget", Cast<ABH_CharacterPlayer>(Actor));

	// Rotate AI to face player
	if (ABH_CharacterPlayer* PlayerTarget = Cast<ABH_CharacterPlayer>(Actor))
	{
		APawn* ControlledPawn = GetPawn();
		if (ControlledPawn)
		{
			Cast<ABH_Enemy>(ControlledPawn)->EnterCombatMode();
			// Get the location of the player
			FVector PlayerLocation = PlayerTarget->GetActorLocation();
			FVector AILocation = ControlledPawn->GetActorLocation();

			// Find the direction the AI should look at
			FRotator LookAtRotation = (PlayerLocation - AILocation).Rotation();

			// Option 1: Instantly rotate towards player (Snap to rotation)
			// ControlledPawn->SetActorRotation(LookAtRotation);

			// Option 2: Smooth rotation towards the player (Interpolating rotation)
			// Uncomment the following code for smooth rotation
			FRotator CurrentRotation = ControlledPawn->GetActorRotation();
			FRotator SmoothedRotation = FMath::RInterpTo(CurrentRotation, LookAtRotation, GetWorld()->GetDeltaSeconds(), 5.0f); // 5.0f is the interpolation speed
			ControlledPawn->SetActorRotation(SmoothedRotation);
			
		}
	}
}


void ABH_EnemyAiController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (ABH_Enemy* const npc = Cast<ABH_Enemy>(InPawn))
	{
		if (UBehaviorTree* const bTree = npc->GetBehaviorTree())
		{
			UBlackboardComponent* b;
			UseBlackboard(bTree->BlackboardAsset, b);
			Blackboard = b;
			RunBehaviorTree(bTree);
		}
	};
}

void ABH_EnemyAiController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		SightConfig->SightRadius = 2000.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 100.f;
		SightConfig->PeripheralVisionAngleDegrees = 500.f;
		SightConfig->SetMaxAge(5.f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ABH_EnemyAiController::OnTargetDetected);

		GetPerceptionComponent()->ConfigureSense((*SightConfig));

		// AAIController::SetGenericTeamId(FGenericTeamId(TeamId));
	}
}

void ABH_EnemyAiController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (auto* const ch = Cast<ABH_CharacterPlayer>(Actor)) 
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			
			Cast<ABH_Enemy>(GetPawn())->EnterCombatMode();
			GetBlackboardComponent()->SetValueAsObject("PlayerTarget", ch);
			GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Green,FString::Printf(TEXT("Player Seen")));
		}
		else
		{
			Cast<ABH_Enemy>(GetPawn())->ExitCombatMode();
			GetBlackboardComponent()->SetValueAsObject("PlayerTarget", nullptr);
			GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,FString::Printf(TEXT("Player Lost")));
		}
		GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
	}
}
