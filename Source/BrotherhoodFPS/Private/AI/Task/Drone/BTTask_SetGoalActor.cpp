// Copyright Chukwuyenum Opone @officialyenum 2024


#include "AI/Task/Drone/BTTask_SetGoalActor.h"

#include "AI/Controller/BH_DroneAiController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pawn/BH_Drone.h"

UBTTask_SetGoalActor::UBTTask_SetGoalActor(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Set Goal Actor");
}

EBTNodeResult::Type UBTTask_SetGoalActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const cont = Cast<ABH_DroneAiController>(OwnerComp.GetAIOwner()))
	{
		if (auto* const bc = OwnerComp.GetBlackboardComponent())
		{
			if (auto* npc = Cast<ABH_Drone>(cont->GetPawn()))
			{
				// get the current patrol path in vector from the npc - this is the local to the patron path actor
				auto const Point = npc->GetGoalActor();
				
				bc->SetValueAsObject(GoalActorLocationKey.SelectedKeyName, Point);
				// finish with success
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	};
	return EBTNodeResult::Failed;
}
