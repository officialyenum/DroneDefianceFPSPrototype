// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Player/BH_PlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"

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
}

void ABH_PlayerController::MoveForward(const FInputActionValue& InputActionValue)
{
	const float InputAxis = InputActionValue.Get<float>();
	if (APawn* ControlledPawn = GetPawn<APawn>())
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
}

void ABH_PlayerController::AimOn(const FInputActionValue& InputActionValue)
{
}

void ABH_PlayerController::AimOff(const FInputActionValue& InputActionValue)
{
}

void ABH_PlayerController::Jump(const FInputActionValue& InputActionValue)
{
	if (ACharacter* ControlledPawn = GetPawn<ACharacter>())
	{
		ControlledPawn->Jump();
	}
}
