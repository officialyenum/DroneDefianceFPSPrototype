// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "BH_CharacterBase.h"
#include "BH_Friendly.generated.h"

UCLASS()
class BROTHERHOODFPS_API ABH_Friendly : public ABH_CharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABH_Friendly();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void ApplyDamageToEnemy(AActor* Actor) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
