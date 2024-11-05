// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ShootTarget.generated.h"

/**
 * 
 */
UCLASS()
class BROTHERHOODFPS_API UBTTask_ShootTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTTask_ShootTarget(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
