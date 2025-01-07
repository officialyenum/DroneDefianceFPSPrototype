// Copyright Chukwuyenum Opone @officialyenum 2024


#include "AI/Task/Enemy/BTTask_ShootTarget.h"

#include "AI/Controller/BH_EnemyAiController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/BH_CharacterPlayer.h"
#include "Character/BH_Enemy.h"

UBTTask_ShootTarget::UBTTask_ShootTarget(FObjectInitializer const& ObjectInitializer)
{
	
	NodeName = TEXT("Shoot Target");
}

EBTNodeResult::Type UBTTask_ShootTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	if (OwnerComp.GetAIOwner() == nullptr) return EBTNodeResult::Failed;
	ICombatInterface* Char = Cast<ICombatInterface>(OwnerComp.GetAIOwner()->GetPawn());
	if(Char == nullptr) return EBTNodeResult::Failed;
	Char->Fire();

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
