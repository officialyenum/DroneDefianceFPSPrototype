// Copyright Chukwuyenum Opone @officialyenum 2024


#include "AI/Task/Enemy/BTTask_FindRandomLocation.h"

#include "NavigationSystem.h"
#include "AI/Controller/BH_EnemyAiController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/BH_CharacterBase.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Random Location in Nav Mesh");
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	if (OwnerComp.GetAIOwner() == nullptr) return EBTNodeResult::Failed;
	APawn* Char = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if(Char == nullptr) return EBTNodeResult::Failed;
	if (auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
	{
		FNavLocation Loc;
		if (NavSys->GetRandomPointInNavigableRadius(Char->GetActorLocation(), SearchRadius, Loc))
		{	
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Loc.Location);
		}
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
