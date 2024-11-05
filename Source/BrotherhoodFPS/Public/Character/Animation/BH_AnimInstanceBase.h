// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BH_AnimInstanceBase.generated.h"

/**
 * 
 */
UCLASS()
class BROTHERHOODFPS_API UBH_AnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	UBH_AnimInstanceBase();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Generic")
	bool IsAiming;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Generic")
	bool IsMoving;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Generic")
<<<<<<< HEAD
=======
	bool IsHoldingPistol;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Generic")
>>>>>>> main
	bool IsSprinting;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Generic")
	float Speed;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
};
