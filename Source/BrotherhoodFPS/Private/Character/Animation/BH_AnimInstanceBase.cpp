﻿// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Character/Animation/BH_AnimInstanceBase.h"

#include "Kismet/BlueprintTypeConversions.h"

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
	// float VectorLength = Velocity.Size();
	// IsMoving = VectorLength > 0 ? true : false;
}
