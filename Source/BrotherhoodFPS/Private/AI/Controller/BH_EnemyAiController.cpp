﻿// Copyright Chukwuyenum Opone @officialyenum 2024


#include "AI/Controller/BH_EnemyAiController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/BH_CharacterPlayer.h"
#include "Perception/AIPerceptionTypes.h"

#include "Character/BH_Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"


ABH_EnemyAiController::ABH_EnemyAiController(FObjectInitializer const& ObjectInitializer)
{
	SetupPerceptionSystem();
}

void ABH_EnemyAiController::BeginPlay()
{
	Super::BeginPlay();

	APawn* PawnPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
	// SetFocus(PawnPlayer);

	if (GetBehaviourTree() !=nullptr)
	{
		RunBehaviorTree(GetBehaviourTree());
		if (PawnPlayer && GetPawn())
		{
			GetBlackboardComponent()->SetValueAsObject(TEXT("Player"), PawnPlayer);
			GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
		}
	}
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

bool ABH_EnemyAiController::IsDead() const
{
	ABH_Enemy* ControlledCharacter = Cast<ABH_Enemy>(GetPawn());
	if (ControlledCharacter != nullptr)
	{
		return ControlledCharacter->IsDead();
	}
	return true;
}


void ABH_EnemyAiController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	APawn* PawnPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
	if (ABH_Enemy* const npc = Cast<ABH_Enemy>(InPawn))
	{
		if (UBehaviorTree* const bTree = GetBehaviourTree())
		{
			UBlackboardComponent* b;
			UseBlackboard(bTree->BlackboardAsset, b);
			Blackboard = b;
			RunBehaviorTree(bTree);
			if (PawnPlayer && npc)
			{
				GetBlackboardComponent()->SetValueAsObject(TEXT("Player"), PawnPlayer);
				GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), npc->GetActorLocation());
			}
		}
	};
}

void ABH_EnemyAiController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	if (SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		SightConfig->SightRadius = 4000.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 200.f;
		SightConfig->PeripheralVisionAngleDegrees = 50.f;
		SightConfig->SetMaxAge(0.f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = -1.0f;
		SightConfig->NearClippingRadius = 200.0f;
		SightConfig->PointOfViewBackwardOffset = 200.0f;
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
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ABH_EnemyAiController::OnTargetDetected);

		GetPerceptionComponent()->ConfigureSense((*SightConfig));

		// AAIController::SetGenericTeamId(FGenericTeamId(TeamId));
	}
}

void ABH_EnemyAiController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (ABH_CharacterPlayer* const ch = Cast<ABH_CharacterPlayer>(Actor)) 
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			Cast<ABH_Enemy>(GetPawn())->EnterCombatMode();
			GetBlackboardComponent()->SetValueAsObject("Player", ch);
			GetBlackboardComponent()->SetValueAsVector("LastKnownLocation", ch->GetActorLocation());
			GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Green,FString::Printf(TEXT("Player Seen")));
		}
		else
		{
			Cast<ABH_Enemy>(GetPawn())->ExitCombatMode();
			GetBlackboardComponent()->ClearValue("Player");
			GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,FString::Printf(TEXT("Player Lost")));
		}
	}
}
