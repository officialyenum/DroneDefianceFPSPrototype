// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Character/Animation/BH_AnimInstanceBase.h"

#include "Kismet/BlueprintTypeConversions.h"
#include "Kismet/KismetMathLibrary.h"

UBH_AnimInstanceBase::UBH_AnimInstanceBase()
{
	IsAiming=false;
	IsMoving=false;
	IsSprinting=false;
	Speed=0.f;
}

void UBH_AnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// FVector Velocity = TryGetPawnOwner()->GetVelocity();
	// Speed = Velocity.Size();
	// IsMoving = Speed > 0 ? true : false;
	// IsSprinting = Speed > 600 ? true : false;
}
