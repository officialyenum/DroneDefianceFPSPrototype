// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "BH_CharacterSandBox.h"
#include "BH_CharacterPlayerSandBox.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class BROTHERHOODFPS_API ABH_CharacterPlayerSandBox : public ABH_CharacterSandBox
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ABH_CharacterPlayerSandBox();
	/**Combat Handlers*/
	virtual void HandleHealthDamaged(float NewHealth, float NewMaxHealth, float HealthChange) override;
	virtual void HandleHealthLow(float CurrentHealth) override;
	virtual void HandleHealthDead(AController* causer) override;
	virtual void HandleShieldDamaged(float NewShield, float MaxShield, float ShieldChange) override;
	virtual void HandleShieldDestroyed(AController* causer) override;
	virtual void HandleCartridgeEmpty(FString Message) override;
	virtual void HandleReloadStart() override;
	virtual void HandleReloadEnd() override;
	/**Combat Handlers*/
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Move(const struct FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);
	void TurnRight(const FInputActionValue& Value);
	void SprintStart(const FInputActionValue& Value);
	void SprintEnd(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE UCameraComponent* GetFollowCamera() const { return  FollowCamera; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta=(AllowPrivateAccess= "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess= "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<class UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<class UInputAction> LookUpAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<class UInputAction> TurnRightAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<class UInputAction> SprintAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<class UInputAction> InteractAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<class UInputAction> JumpAction;
	
	UPROPERTY(EditAnywhere, Category="Generic")
	float RotationRate = 10.0f;

	UPROPERTY(EditAnywhere, Category="Generic")
	FVector RespawnLocation;
};
