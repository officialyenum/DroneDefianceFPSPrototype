// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Player/BH_PlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Character/BH_CharacterPlayer.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

ABH_PlayerController::ABH_PlayerController()
{
	bReplicates = true;
}

void ABH_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(BHContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem)
	Subsystem->AddMappingContext(BHContext, 0);

	
}

void ABH_PlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void ABH_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ABH_PlayerController::MoveForward);
	EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ABH_PlayerController::MoveRight);
	EnhancedInputComponent->BindAction(LookUpAction, ETriggerEvent::Triggered, this, &ABH_PlayerController::LookUp);
	EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ABH_PlayerController::Turn);
	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &ABH_PlayerController::AimOn);
	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ABH_PlayerController::AimOff);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ABH_PlayerController::Jump);
	EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ABH_PlayerController::Shoot);
	EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &ABH_PlayerController::FiringOn);
	EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &ABH_PlayerController::FiringOff);
	EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &ABH_PlayerController::EquipWeapon);
	EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &ABH_PlayerController::Reload);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ABH_PlayerController::Sprint);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ABH_PlayerController::StopSprint);

}

void ABH_PlayerController::MoveForward(const FInputActionValue& InputActionValue)
{
	const float InputAxis = InputActionValue.Get<float>();
	if (ABH_CharacterPlayer* ControlledPawn = GetPawn<ABH_CharacterPlayer>())
	{
		const FVector ForwardDirection = ControlledPawn->GetActorForwardVector();
		ControlledPawn->AddMovementInput(ForwardDirection,InputAxis);
	}
}

void ABH_PlayerController::MoveRight(const FInputActionValue& InputActionValue)
{
	const float InputAxis = InputActionValue.Get<float>();
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		const FVector RightDirection = ControlledPawn->GetActorRightVector();
		ControlledPawn->AddMovementInput(RightDirection,InputAxis);
	}
}

void ABH_PlayerController::LookUp(const FInputActionValue& InputActionValue)
{
	const float InputAxis = InputActionValue.Get<float>();
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddControllerPitchInput(InputAxis);
	}
}

void ABH_PlayerController::Turn(const FInputActionValue& InputActionValue)
{
	const float InputAxis = InputActionValue.Get<float>();
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddControllerYawInput(InputAxis);
	}
}

void ABH_PlayerController::Shoot(const FInputActionValue& InputActionValue)
{
	if (ABH_CharacterPlayer* ControlledPawn = GetPawn<ABH_CharacterPlayer>())
	{
		if (ControlledPawn->Ammo <= 0)
		{
			if (ControlledPawn->Cartridge > 0)
			{
				ControlledPawn->ReloadWeapon();
			}
			else
			{
				ControlledPawn->TriggerOutOfAmmoMessageInUI();
			}
		}
		else
		{
			ControlledPawn->FireWeapon();
		}
	}
}

void ABH_PlayerController::AimOn(const FInputActionValue& InputActionValue)
{
	if (ABH_CharacterPlayer* ControlledPawn = GetPawn<ABH_CharacterPlayer>())
	{
		ControlledPawn->AnimBP->IsAiming = true;
		if (auto fpsCam = ControlledPawn->Camera)
		{
			fpsCam->SetFieldOfView(60.0f);
		}
	}
}

void ABH_PlayerController::AimOff(const FInputActionValue& InputActionValue)
{
	if (ABH_CharacterPlayer* ControlledPawn = GetPawn<ABH_CharacterPlayer>())
	{
		ControlledPawn->AnimBP->IsAiming = false;
		if (auto fpsCam = ControlledPawn->Camera)
		{
			fpsCam->SetFieldOfView(90.0f);
		}
	}
}

void ABH_PlayerController::FiringOn(const FInputActionValue& InputActionValue)
{
	if (ABH_CharacterPlayer* ControlledPawn = GetPawn<ABH_CharacterPlayer>())
	{
		ControlledPawn->FireButtonPressed = true;
	}
}

void ABH_PlayerController::FiringOff(const FInputActionValue& InputActionValue)
{
	if (ABH_CharacterPlayer* ControlledPawn = GetPawn<ABH_CharacterPlayer>())
	{
		ControlledPawn->FireButtonPressed = false;
	}
}

void ABH_PlayerController::Sprint(const FInputActionValue& InputActionValue)
{
	if (ABH_CharacterPlayer* ControlledPawn = GetPawn<ABH_CharacterPlayer>())
	{
		ControlledPawn->GetCharacterMovement()->MaxWalkSpeed = 1500.0f;
		ControlledPawn->AnimBP->IsSprinting = true;
	}
}

void ABH_PlayerController::StopSprint(const FInputActionValue& InputActionValue)
{
	if (ABH_CharacterPlayer* ControlledPawn = GetPawn<ABH_CharacterPlayer>())
	{
		ControlledPawn->GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		ControlledPawn->AnimBP->IsSprinting = false;
	}
}

void ABH_PlayerController::Jump(const FInputActionValue& InputActionValue)
{
	if (ACharacter* ControlledPawn = GetPawn<ACharacter>())
	{
		ControlledPawn->Jump();
	}
}

void ABH_PlayerController::EquipWeapon(const FInputActionValue& InputActionValue)
{
	
	const float InputAxis = InputActionValue.Get<float>();
	if (ABH_CharacterPlayer* ControlledPawn = GetPawn<ABH_CharacterPlayer>())
	{
		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Blue,FString::Printf(TEXT("Equipping New Weapon")));
	}
}

void ABH_PlayerController::Reload(const FInputActionValue& InputActionValue)
{
	if (ABH_CharacterPlayer* ControlledPawn = GetPawn<ABH_CharacterPlayer>())
	{
		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Blue,FString::Printf(TEXT("Reloading Weapon")));
		ControlledPawn->ReloadWeapon();
	}
}
