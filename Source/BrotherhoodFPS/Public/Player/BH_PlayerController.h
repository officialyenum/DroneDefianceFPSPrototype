// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BH_PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

/**
 * 
 */
UCLASS()
class BROTHERHOODFPS_API ABH_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABH_PlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Camera Settings")
	float InterpSpeed = 10.0f;

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> BHContext;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveForwardAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveRightAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> LookUpAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> TurnAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ShootAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> AimAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ReloadAction;
	UPROPERTY(EditAnywhere, Category="Input")
<<<<<<< HEAD
	TObjectPtr<UInputAction> SprintAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> QuitGameAction;
=======
	TObjectPtr<UInputAction> EquipAction;
	TObjectPtr<UInputAction> SprintAction;
>>>>>>> main

	UFUNCTION(BlueprintCallable, Category="Player Actions")
	void MoveForward(const struct FInputActionValue& InputActionValue);
	UFUNCTION(BlueprintCallable, Category="Player Actions")
	void MoveRight(const struct FInputActionValue& InputActionValue);
	UFUNCTION(BlueprintCallable, Category="Player Actions")
	void LookUp(const struct FInputActionValue& InputActionValue);
	UFUNCTION(BlueprintCallable, Category="Player Actions")
	void Turn(const struct FInputActionValue& InputActionValue);
	UFUNCTION(BlueprintCallable, Category="Player Actions")
	void Shoot(const struct FInputActionValue& InputActionValue);
	UFUNCTION(BlueprintCallable, Category="Player Actions")
	void AimOn(const struct FInputActionValue& InputActionValue);
	UFUNCTION(BlueprintCallable, Category="Player Actions")
	void AimOff(const struct FInputActionValue& InputActionValue);
	UFUNCTION(BlueprintCallable, Category="Player Actions")
	void FiringOn(const struct FInputActionValue& InputActionValue);
	UFUNCTION(BlueprintCallable, Category="Player Actions")
	void FiringOff(const struct FInputActionValue& InputActionValue);
	UFUNCTION(BlueprintCallable, Category="Player Actions")
	void Sprint(const struct FInputActionValue& InputActionValue);
	UFUNCTION(BlueprintCallable, Category="Player Actions")
	void StopSprint(const struct FInputActionValue& InputActionValue);
	UFUNCTION(BlueprintCallable, Category="Player Actions")
	void Jump(const struct FInputActionValue& InputActionValue);
	UFUNCTION(BlueprintCallable, Category="Player Actions")
<<<<<<< HEAD
	void Reload(const struct FInputActionValue& InputActionValue);
	UFUNCTION(BlueprintCallable, Category="Player Actions")
	void QuitGame(const struct FInputActionValue& InputActionValue);
	
=======
	void EquipWeapon(const struct FInputActionValue& InputActionValue);
	UFUNCTION(BlueprintCallable, Category="Player Actions")
	void Reload(const struct FInputActionValue& InputActionValue);
>>>>>>> main
};
