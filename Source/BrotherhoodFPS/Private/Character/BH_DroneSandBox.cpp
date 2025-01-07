// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Character/BH_DroneSandBox.h"

#include "AI/Controller/BH_DroneAiController.h"
#include "Character/BH_CharacterPlayerSandBox.h"
#include "Component/HealthSystem.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"


// Sets default values
ABH_DroneSandBox::ABH_DroneSandBox()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ABH_DroneAiController::StaticClass();
	
	SphereCollision =  CreateDefaultSubobject<USphereComponent>("ExplosionSphere");
	GetSphereCollision()->SetSphereRadius(100.f);
	GetSphereCollision()->SetupAttachment(RootComponent);
	GetSphereCollision()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Overlap);
	GetSphereCollision()->SetGenerateOverlapEvents(true);

	
	GetCapsuleComponent()->InitCapsuleSize(60.f, 96.0f);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Overlap);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(false);
	GetMesh()->SetRelativeLocation(FVector(0.f,0.f,-91.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
	SkeletalMeshAsset(TEXT("/Game/Assets/Characters/ParagonLtBelica/Characters/Heroes/Belica/Meshes/Belica_Drone/Meshes/Belica_Drone"));
	if(SkeletalMeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshAsset.Object);
	}

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	
	HealthSystem = CreateDefaultSubobject<UHealthSystem>("Health Manager");
	
	SetupStimulusSource();
}

void ABH_DroneSandBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	GetSphereCollision()->OnComponentBeginOverlap.AddDynamic(this,&ABH_DroneSandBox::ExplosionSphereBeginOverlap);
	DynamicDroneMaterial = GetMesh()->CreateDynamicMaterialInstance(0,GetMesh()->GetMaterial(0));
	GetMesh()->SetMaterial(0,DynamicDroneMaterial);
	ExitChaseMode();
	if (HealthSystem)
	{
		HealthSystem->OnHealthDamaged.AddDynamic(this, &ABH_DroneSandBox::HandleHealthDamaged);
		HealthSystem->OnHealthDead.AddDynamic(this, &ABH_DroneSandBox::HandleHealthDead);
	}
}

void ABH_DroneSandBox::ExplosionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,FString::Printf(TEXT("Explosion Sphere Overlapped ")));
	if (ABH_CharacterPlayerSandBox* Player = Cast<ABH_CharacterPlayerSandBox>(OtherActor))
	{
		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Green,FString::Printf(TEXT("Player Overlapped Explosion Sphere")));
		TSubclassOf<UDamageType> DamageType;
		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Blue,FString::Printf(TEXT("Damage Applied To BP_Player")));
		UGameplayStatics::ApplyDamage(Player,BulletDamage,GetController(),this,DamageType);
		Explode();
	}
}

void ABH_DroneSandBox::Explode()
{
	FVector Loc = GetActorLocation();
	FRotator Rot = GetActorRotation();
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplodeFX,Loc,Rot,FVector(1));
	UGameplayStatics::PlaySoundAtLocation(this,ExplosionSound,Loc,Rot);

	//TODO: Update Game mode that drone died
	Destroy();
}

void ABH_DroneSandBox::EnterChaseMode()
{
	GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,FString::Printf(TEXT("Entered Chase Mode ")));
	GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
	ChangeDroneColor(RageColor);
}

void ABH_DroneSandBox::ExitChaseMode()
{
	GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Blue,FString::Printf(TEXT("Exited Chase Mode ")));
	GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
	ChangeDroneColor(CoolColor);
}

void ABH_DroneSandBox::ChangeDroneColor(FLinearColor NewColor)
{
	if (DynamicDroneMaterial)
	{
		DynamicDroneMaterial->SetVectorParameterValue("TeamColor", NewColor);
	}
}

// Called when the game starts or when spawned
void ABH_DroneSandBox::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetActorLocation();
}

void ABH_DroneSandBox::Fire()
{
}

// Called every frame
void ABH_DroneSandBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ABH_DroneSandBox::IsDead() const
{
	return !HealthSystem->HasHealth();
}

void ABH_DroneSandBox::HandleHealthDamaged(float NewHealth, float MaxHealth, float HealthChange)
{
	GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("Handle Player Damage"));
}

void ABH_DroneSandBox::HandleHealthLow(float CurrentHealth)
{
}

void ABH_DroneSandBox::HandleShieldDamaged(float NewShield, float MaxShield, float ShieldChange)
{
}

void ABH_DroneSandBox::HandleShieldDestroyed(AController* causer)
{
}

void ABH_DroneSandBox::HandleCartridgeEmpty(FString Message)
{
}

void ABH_DroneSandBox::HandleReloadStart()
{
}

void ABH_DroneSandBox::HandleReloadEnd()
{
}

void ABH_DroneSandBox::HandleHealthDead(AController* causer)
{
	GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("Handle Player Death"));
}

void ABH_DroneSandBox::SetupStimulusSource()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	if (StimulusSource)
	{
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Hearing>());
		StimulusSource->RegisterWithPerceptionSystem();
	}
}

