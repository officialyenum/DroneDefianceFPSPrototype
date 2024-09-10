// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "BH_EnemyAiController.generated.h"

class UAISenseConfig_Sight;

UCLASS()
class BROTHERHOODFPS_API ABH_EnemyAiController : public AAIController
{
	GENERATED_BODY()

public:
	explicit ABH_EnemyAiController(FObjectInitializer const& ObjectInitializer);
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	
	UPROPERTY(BlueprintReadWrite, Category = AI)
	int32 TeamId = 0;
	
	UPROPERTY(VisibleAnywhere, Category = AI)
	UAIPerceptionComponent* UaiPerceptionComponent;
	UPROPERTY(VisibleAnywhere, Category = AI)
	UAISenseConfig_Sight* SightConfig;

	UFUNCTION()
	void OnEnemyHit(AActor* Actor);
protected:
	virtual void OnPossess(APawn* InPawn) override;
private:

	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);
};
