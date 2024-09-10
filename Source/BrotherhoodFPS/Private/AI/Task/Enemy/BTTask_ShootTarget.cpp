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
	if (auto* const cont = Cast<ABH_EnemyAiController>(OwnerComp.GetAIOwner()))
	{
		if (auto* const bc = OwnerComp.GetBlackboardComponent())
		{
			if (auto* npc = Cast<ABH_Enemy>(cont->GetPawn()))
			{
				FName CKeyName = GetSelectedBlackboardKey();
				if (auto* playerChar = Cast<ABH_CharacterPlayer>(bc->GetValueAsObject(GetSelectedBlackboardKey())))
				{
					// Get the location of the player
					FVector PlayerLocation = playerChar->GetActorLocation();
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
					npc->FireWeapon();
				}
				// finish with success
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	};
	return EBTNodeResult::Failed;
}
