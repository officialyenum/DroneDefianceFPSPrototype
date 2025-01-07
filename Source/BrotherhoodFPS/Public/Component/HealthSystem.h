// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthDeadSignature, AController*, causer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthLowSignature, float, CurrentHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHealthDamagedSignature, float, NewHealth, float, MaxHealth, float, HealthChange);


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FStrengthUsedSignature, float, NewHealth, float, MaxHealth, float, HealthChange);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FShieldDamagedSignature, float, NewShield, float, MaxShield, float, ShieldChange);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShieldDestroyedSignature, AController*, causer);

USTRUCT(BlueprintType)
struct FCharacterAttr : public  FTableRowBase
{
	GENERATED_BODY()
public:
	FCharacterAttr()
	{
	}
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character Params",meta=(AllowPrivateAccess = "true"))
	float Strength = 100.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character Params",meta=(AllowPrivateAccess = "true"))
	float MaxStrength = 100.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character Params",meta=(AllowPrivateAccess = "true"))
	float Health = 100.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character Params",meta=(AllowPrivateAccess = "true"))
	float MaxHealth = 100.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character Params",meta=(AllowPrivateAccess = "true"))
	float Shield = 100.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character Params",meta=(AllowPrivateAccess = "true"))
	float MaxShield = 100.0f;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BROTHERHOODFPS_API UHealthSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthSystem();

	// Health Signatures
	UPROPERTY(BlueprintAssignable)
	FHealthDeadSignature OnHealthDead;
	UPROPERTY(BlueprintAssignable)
	FHealthLowSignature OnHealthLow;
	UPROPERTY(BlueprintAssignable)
	FHealthDamagedSignature OnHealthDamaged;
	
	// Strength Signatures
	UPROPERTY(BlueprintAssignable)
	FStrengthUsedSignature OnStrengthUsed;
	
	// Shield Signatures
	UPROPERTY(BlueprintAssignable)
	FShieldDamagedSignature OnShieldDamaged;
	UPROPERTY(BlueprintAssignable)
	FShieldDestroyedSignature OnShieldDestroyed;
	
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable, Category="Health Component|Health")
	void InitHealth();
	UFUNCTION(BlueprintCallable, Category="Health Component|Health")
	float AddHealth(float HealthToAdd);
	UFUNCTION(BlueprintCallable, Category="Health Component|Health")
	float ConsumeHealth(float HealthToConsume);
	UFUNCTION(BlueprintPure, Category="Health Component|Health")
	float GetHealthPercentage() const;
	UFUNCTION(BlueprintPure)
	bool HasHealth() const;

	
	UFUNCTION(BlueprintCallable, Category="Health Component|Strength")
	void InitStrength();
	UFUNCTION(BlueprintCallable, Category="Health Component|Strength")
	float AddStrength(float StrengthToAdd);
	UFUNCTION(BlueprintCallable, Category="Health Component|Strength")
	float ConsumeStrength(float StrengthToConsume);
	UFUNCTION(BlueprintPure, Category="Health Component|Strength")
	float GetStrengthPercentage() const;
	UFUNCTION(BlueprintPure)
	bool HasStrength() const;
	
	UFUNCTION(BlueprintCallable, Category="Health Component|Shield")
	void InitShield();
	UFUNCTION(BlueprintCallable, Category="Health Component|Shield")
	float AddShield(float ShieldToAdd);
	UFUNCTION(BlueprintCallable, Category="Health Component|Shield")
	float ConsumeShield(float ShieldToConsume);
	UFUNCTION(BlueprintPure, Category="Health Component|Shield")
	float GetShieldPercentage() const;
	UFUNCTION(BlueprintPure)
	bool HasShield() const;
	// Damage Manager
	UFUNCTION()
	void DamageTaken(AActor* damagedActor, float damage, const UDamageType* dmgType, AController* instigator, AActor* causer);
	FCharacterAttr GetCharacterAttr() { return CharacterAttr; };

private:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Health Component|Shield",meta=(AllowPrivateAccess = "true"))
	FCharacterAttr CharacterAttr;
};
