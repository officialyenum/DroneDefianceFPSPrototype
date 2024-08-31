// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Pawn/BH_Drone.h"

#include "AIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/BH_CharacterPlayer.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ABH_Drone::ABH_Drone()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	DefaultSceneRoot->SetupAttachment(RootComponent);
	DroneMesh = CreateDefaultSubobject<USkeletalMeshComponent>("DroneMesh");
	DroneMesh->SetupAttachment(DefaultSceneRoot);
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(DefaultSceneRoot);
	Sphere->SetGenerateOverlapEvents(true);
	ExplosionSphere = CreateDefaultSubobject<USphereComponent>("ExplosionSphere");
	ExplosionSphere->SetupAttachment(DefaultSceneRoot);
	ExplosionSphere->SetGenerateOverlapEvents(true);
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("FloatingPawnMovement");
}

// Called when the game starts or when spawned
void ABH_Drone::BeginPlay()
{
	Super::BeginPlay();
	ExplosionSphere->OnComponentBeginOverlap.AddDynamic(this,&ABH_Drone::ExplosionSphereBeginOverlap);
	Sphere->OnComponentBeginOverlap.AddDynamic(this,&ABH_Drone::SphereBeginOverlap);
	OnTakeAnyDamage.AddDynamic(this,&ABH_Drone::TakeHitDamage);
	
}

void ABH_Drone::SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,FString::Printf(TEXT("Sphere Overlapped")));
	if (ABH_CharacterPlayer* Player = Cast<ABH_CharacterPlayer>(OtherActor))
	{
		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Green,FString::Printf(TEXT("Player Overlapped Sphere")));
		MoveToGoalTarget(Player);
	}
}

void ABH_Drone::ExplosionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,FString::Printf(TEXT("Explosion Sphere Overlapped ")));
	if (ABH_CharacterPlayer* Player = Cast<ABH_CharacterPlayer>(OtherActor))
	{
		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Green,FString::Printf(TEXT("Player Overlapped Explosion Sphere")));
		ApplyDamageToEnemy(Player);
		Explode();
	}
}

void ABH_Drone::ApplyDamageToEnemy(AActor* Actor)
{
	if (ABH_CharacterPlayer* Enemy = Cast<ABH_CharacterPlayer>(Actor))
	{
		TSubclassOf<UDamageType> DamageType;
		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Blue,FString::Printf(TEXT("Damage Applied To BP_Player")));
		UGameplayStatics::ApplyDamage(Enemy,BulletDamage,GetController(),this,DamageType);
	}
}

void ABH_Drone::TakeHitDamage(AActor* DamagedActor, float Damage,const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	
	GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,FString::Printf(TEXT("Drone Took Damage")));
	float NewHealth = Health - Damage;
	Health = FMath::Clamp(NewHealth, 0, MaxHealth);
	if (Health <= 0)
	{
		Explode();
	}
	ChangeDroneColor(true);
}

void ABH_Drone::Explode()
{
	FVector Loc = GetActorLocation();
	FRotator Rot = GetActorRotation();
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplodeFX,Loc,Rot,FVector(1));
	UGameplayStatics::PlaySoundAtLocation(this,ExplosionSound,Loc,Rot);

	//TODO: Update Game mode that drone died

	Destroy();
}

void ABH_Drone::ChangeDroneColor(bool bIsInRageMode)
{
	check(DynamicDroneMaterial);
	if (!bIsInRageMode)
	{
		//DynamicDroneMaterial->SetVectorParameterValue(FName("TeamColor"), CoolColor);
	}
	else
	{
		//DynamicDroneMaterial->SetVectorParameterValue(FName("TeamColor"), RageColor);
	}
}

void ABH_Drone::MoveToGoalTarget(AActor* NewGoalTarget)
{
	GoalActor = NewGoalTarget;
	// Get the AI controller for this character
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		// Move to the goal actor
		UAIBlueprintHelperLibrary::SimpleMoveToActor(AIController, GoalActor);
	}
}

// Called every frame
void ABH_Drone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(GoalActor))
	{
		FRotator CurrentRot = GetActorRotation();
		FRotator NextRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GoalActor->GetActorLocation());
		SetActorRotation(UKismetMathLibrary::RInterpTo(CurrentRot, NextRot, DeltaTime,5.0f));
	}
}

