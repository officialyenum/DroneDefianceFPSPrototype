// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "BH_DroneAiController.generated.h"

class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;

UCLASS()
class BROTHERHOODFPS_API ABH_DroneAiController : public AAIController
{
	GENERATED_BODY()

public: 
	explicit ABH_DroneAiController(FObjectInitializer const& ObjectInitializer);
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	
	UPROPERTY(BlueprintReadWrite, Category = AI)
	int32 TeamId = 0;
	
	UPROPERTY(VisibleAnywhere, Category = AI)
	UAIPerceptionComponent* UaiPerceptionComponent;
	UPROPERTY(VisibleAnywhere, Category = AI)
	UAISenseConfig_Sight* SightConfig;
	UPROPERTY(VisibleAnywhere, Category = AI)
	UAISenseConfig_Hearing* HearingConfig;
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
private:

	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);
};
