// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Player/BH_PlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Character/BH_CharacterPlayer.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
<<<<<<< HEAD
#include "Kismet/GameplayStatics.h"
=======
>>>>>>> main

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
<<<<<<< HEAD
	EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &ABH_PlayerController::Reload);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ABH_PlayerController::Sprint);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ABH_PlayerController::StopSprint);
	EnhancedInputComponent->BindAction(QuitGameAction, ETriggerEvent::Started, this, &ABH_PlayerController::QuitGame);
=======
	EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &ABH_PlayerController::EquipWeapon);
	EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &ABH_PlayerController::Reload);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ABH_PlayerController::Sprint);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ABH_PlayerController::StopSprint);

>>>>>>> main
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
<<<<<<< HEAD
		ControlledPawn->AnimBP->IsSprinting = false;

		// Assuming the character has a camera component and default FOV is stored
		if (UCameraComponent* CameraComponent = ControlledPawn->Camera)
		{
			// Smoothly zoom in by decreasing the FOV
			CameraComponent->SetFieldOfView(FMath::FInterpTo(CameraComponent->FieldOfView, 45.0f, GetWorld()->GetDeltaSeconds(), InterpSpeed)); // Zoom in, target FOV is 45
=======
		if (auto fpsCam = ControlledPawn->Camera)
		{
			fpsCam->SetFieldOfView(60.0f);
>>>>>>> main
		}
	}
}

void ABH_PlayerController::AimOff(const FInputActionValue& InputActionValue)
{
	if (ABH_CharacterPlayer* ControlledPawn = GetPawn<ABH_CharacterPlayer>())
	{
		ControlledPawn->AnimBP->IsAiming = false;
<<<<<<< HEAD

		// Reset the camera's FOV to its default value
		if (UCameraComponent* CameraComponent = ControlledPawn->Camera)
		{
			// Smoothly return to the default FOV
			CameraComponent->SetFieldOfView(FMath::FInterpTo(CameraComponent->FieldOfView, ControlledPawn->DefaultFOV, GetWorld()->GetDeltaSeconds(), InterpSpeed)); // Reset to default FOV
=======
		if (auto fpsCam = ControlledPawn->Camera)
		{
			fpsCam->SetFieldOfView(90.0f);
>>>>>>> main
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
<<<<<<< HEAD
		if (ControlledPawn->Strength > 40 && ControlledPawn->AnimBP->IsAiming == false)
		{
			ControlledPawn->GetCharacterMovement()->MaxWalkSpeed = 1500.0f;
			ControlledPawn->AnimBP->IsSprinting = true;
		}
=======
		ControlledPawn->GetCharacterMovement()->MaxWalkSpeed = 1500.0f;
		ControlledPawn->AnimBP->IsSprinting = true;
>>>>>>> main
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

<<<<<<< HEAD
=======
void ABH_PlayerController::EquipWeapon(const FInputActionValue& InputActionValue)
{
	
	const float InputAxis = InputActionValue.Get<float>();
	if (ABH_CharacterPlayer* ControlledPawn = GetPawn<ABH_CharacterPlayer>())
	{
		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Blue,FString::Printf(TEXT("Equipping New Weapon")));
	}
}

>>>>>>> main
void ABH_PlayerController::Reload(const FInputActionValue& InputActionValue)
{
	if (ABH_CharacterPlayer* ControlledPawn = GetPawn<ABH_CharacterPlayer>())
	{
<<<<<<< HEAD
		if (ControlledPawn->Cartridge > 0)
		{
			GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Blue,FString::Printf(TEXT("Reloading Weapon")));
			ControlledPawn->ReloadWeapon();
		}
	}
}

void ABH_PlayerController::QuitGame(const FInputActionValue& InputActionValue)
{
	// Check if the world is valid
	if (UWorld* World = GetWorld())
	{
		// Open the "Main_Menu" level by name
		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Blue,FString::Printf(TEXT("Quit Button Pressed")));
		UGameplayStatics::OpenLevel(World, FName("Main_Menu"));
=======
		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Blue,FString::Printf(TEXT("Reloading Weapon")));
		ControlledPawn->ReloadWeapon();
>>>>>>> main
	}
}
