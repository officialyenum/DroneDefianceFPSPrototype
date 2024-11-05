// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "BH_CharacterBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Interaction/CombatInterface.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "BH_Enemy.generated.h"

UCLASS()
class BROTHERHOODFPS_API ABH_Enemy : public ABH_CharacterBase, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABH_Enemy();

	UBehaviorTree* GetBehaviorTree() const;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void EnterCombatMode();
	UFUNCTION()
	void ExitCombatMode();
	
	UFUNCTION()
	virtual void ApplyDamageToEnemy(AActor* Actor) override;
	UFUNCTION()
	virtual void TakeHitDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
							AController* InstigatedBy, AActor* DamageCauser) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* Tree;
	
	UPROPERTY(VisibleAnywhere)
	UAIPerceptionStimuliSourceComponent* StimulusSource;
	void SetupStimulusSource();
};
