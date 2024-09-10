// Copyright Chukwuyenum Opone @officialyenum 2024


#include "AI/Task/Enemy/BTTask_FindRandomLocation.h"

#include "NavigationSystem.h"
#include "AI/Controller/BH_EnemyAiController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Random Location in Nav Mesh");
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ABH_EnemyAiController* const cont = Cast<ABH_EnemyAiController>(OwnerComp.GetAIOwner()))
	{
		if (auto* const npc = cont->GetPawn())
		{
			auto const Origin = npc->GetActorLocation();

			if (auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				FNavLocation Loc;
				if (NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, Loc))
				{	
					// Get the location of the player
					FVector PlayerLocation = Loc;
					FVector AILocation = npc->GetActorLocation();

					// Find the direction the AI should look at
					FRotator LookAtRotation = (PlayerLocation - AILocation).Rotation();

					// Option 1: Instantly rotate towards player (Snap to rotation)
					// Uncomment the following code for quick rotation
					// ControlledPawn->SetActorRotation(LookAtRotation);

					// Option 2: Smooth rotation towards the player (Interpolating rotation)
					FRotator CurrentRotation = npc->GetActorRotation();
					FRotator SmoothedRotation = FMath::RInterpTo(CurrentRotation, LookAtRotation, GetWorld()->GetDeltaSeconds(), 5.0f); // 5.0f is the interpolation speed
					npc->SetActorRotation(SmoothedRotation);
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Loc.Location);
				}
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
