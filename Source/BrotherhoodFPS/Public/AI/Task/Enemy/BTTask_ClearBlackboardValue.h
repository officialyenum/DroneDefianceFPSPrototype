// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ClearBlackboardValue.generated.h"

/**
 * 
 */
UCLASS()
class BROTHERHOODFPS_API UBTTask_ClearBlackboardValue : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_ClearBlackboardValue();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
