// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Character/BH_CharacterPlayerSandBox.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Component/WeaponSystem.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/BH_PlayerController.h"


// Sets default values
ABH_CharacterPlayerSandBox::ABH_CharacterPlayerSandBox()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("Follow Camera");
	FollowCamera->SetupAttachment(GetCapsuleComponent());
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->SetFieldOfView(90.0f);


	GetCapsuleComponent()->InitCapsuleSize(60.f, 96.0f);

	bUseControllerRotationPitch = true;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;

	RespawnLocation = FVector(-1970.000000,6280.000000,98.000100);
	GetMesh()->SetRelativeLocation(FVector(0.f,0.f,-91.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
	SkeletalMeshAsset(TEXT("/Game/Assets/Characters/CH44/Ch44_nonPBR"));
	if(SkeletalMeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshAsset.Object);
	}

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 700.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2048.f;
	
}

void ABH_CharacterPlayerSandBox::HandleHealthDamaged(float NewHealth, float NewMaxHealth, float HealthChange)
{
	Super::HandleHealthDamaged(NewHealth, NewMaxHealth, HealthChange);
}

void ABH_CharacterPlayerSandBox::HandleHealthLow(float CurrentHealth)
{
	Super::HandleHealthLow(CurrentHealth);
}

void ABH_CharacterPlayerSandBox::HandleHealthDead(AController* causer)
{
	Super::HandleHealthDead(causer);
}

void ABH_CharacterPlayerSandBox::HandleShieldDamaged(float NewShield, float MaxShield, float ShieldChange)
{
	Super::HandleShieldDamaged(NewShield, MaxShield, ShieldChange);
}

void ABH_CharacterPlayerSandBox::HandleShieldDestroyed(AController* causer)
{
	Super::HandleShieldDestroyed(causer);
}

void ABH_CharacterPlayerSandBox::HandleCartridgeEmpty(FString Message)
{
	Super::HandleCartridgeEmpty(Message);
}

void ABH_CharacterPlayerSandBox::HandleReloadStart()
{
	Super::HandleReloadStart();
}

void ABH_CharacterPlayerSandBox::HandleReloadEnd()
{
	Super::HandleReloadEnd();
}

// Called when the game starts or when spawned
void ABH_CharacterPlayerSandBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABH_CharacterPlayerSandBox::Move(const FInputActionValue& Value)
{
	const auto MovementVector = Value.Get<FVector2D>();
	if(Controller != nullptr)
	{
		const auto Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
		const auto ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const auto RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection * MovementVector.Y);
		AddMovementInput(RightDirection * MovementVector.X);
	}
}

void ABH_CharacterPlayerSandBox::LookUp(const FInputActionValue& Value)
{
	const float InputAxis = Value.Get<float>();
	AddControllerPitchInput(GetWorld()->GetDeltaSeconds() * RotationRate * InputAxis);
}

void ABH_CharacterPlayerSandBox::TurnRight(const FInputActionValue& Value)
{
	const auto LookAxisVector = Value.Get<float>();
	AddControllerYawInput(GetWorld()->GetDeltaSeconds() * RotationRate * LookAxisVector);
	
}

void ABH_CharacterPlayerSandBox::SprintStart(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 3000.f;
}

void ABH_CharacterPlayerSandBox::SprintEnd(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
}

void ABH_CharacterPlayerSandBox::Interact(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Red, TEXT("Interact Pressed"));
}

// Called every frame
void ABH_CharacterPlayerSandBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABH_CharacterPlayerSandBox::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (ABH_PlayerController* PC = Cast<ABH_PlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	
	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABH_CharacterPlayerSandBox::Move);
		EnhancedInputComponent->BindAction(LookUpAction, ETriggerEvent::Triggered, this, &ABH_CharacterPlayerSandBox::LookUp);
		EnhancedInputComponent->BindAction(TurnRightAction, ETriggerEvent::Triggered, this, &ABH_CharacterPlayerSandBox::TurnRight);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ABH_CharacterPlayerSandBox::SprintStart);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ABH_CharacterPlayerSandBox::SprintEnd);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ABH_CharacterPlayerSandBox::Interact);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
	};
}

