// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Pawn/BH_Drone.h"

#include "AI/Controller/BH_DroneAiController.h"
#include "Character/BH_CharacterPlayer.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AISense_Sight.h"


// Sets default values
ABH_Drone::ABH_Drone()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	
	DroneMesh = CreateDefaultSubobject<USkeletalMeshComponent>("DroneMesh");
	DroneMesh->SetupAttachment(RootComponent);
	
	ExplosionSphere = CreateDefaultSubobject<USphereComponent>("ExplosionSphere");
	ExplosionSphere->SetupAttachment(RootComponent);
	ExplosionSphere->SetGenerateOverlapEvents(true);
	
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("FloatingPawnMovement");

	SetupStimulusSource();
	
	// Default Values
	Health = 50.0f;
	MaxHealth = 50.0f;
	CurrentState = EDroneState::Patrol;
	RageColor = FLinearColor::Red;
	CoolColor = FLinearColor::Blue;
	BulletDamage = 20.0f;
	
}

UBehaviorTree* ABH_Drone::GetBehaviorTree() const
{
	return Tree;
}

// Called when the game starts or when spawned
void ABH_Drone::BeginPlay()
{
	Super::BeginPlay();
	ExplosionSphere->OnComponentBeginOverlap.AddDynamic(this,&ABH_Drone::ExplosionSphereBeginOverlap);
	OnTakeAnyDamage.AddDynamic(this,&ABH_Drone::TakeHitDamage);
	
	DynamicDroneMaterial = DroneMesh->CreateAndSetMaterialInstanceDynamic(0);

	ChangeDroneColor(CoolColor);
	SetDroneState(EDroneState::Patrol);
	FTimerHandle TimerHandle;
	// Bind the InitPatrolPoint function to be called after 2 seconds
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABH_Drone::InitPatrolPoint, 2.0f, false);
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
	ChangeDroneColor(RageColor);
	if(ABH_DroneAiController* DC = Cast<ABH_DroneAiController>(GetController()))
	{
		DC->OnDroneHit(DamageCauser);
	}
}
void ABH_Drone::InitPatrolPoint()

{
	TArray<AActor*> PatrolPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABH_PatrolPoint::StaticClass(), PatrolPoints);

	if (PatrolPoints.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, PatrolPoints.Num() - 1);
		NextPatrolPoint = Cast<ABH_PatrolPoint>(PatrolPoints[RandomIndex]);
		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Blue,FString::Printf(TEXT("NextPatrolPoint Initialized")));
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,FString::Printf(TEXT("NextPatrolPoint Not Initialized")));
}



ABH_PatrolPoint* ABH_Drone::GoToNextPatrolPoint()
{
	NextPatrolPoint = Cast<ABH_PatrolPoint>(NextPatrolPoint->NextLocation);
	return NextPatrolPoint;
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

void ABH_Drone::ChangeDroneColor(FLinearColor NewColor)
{
	if (DynamicDroneMaterial)
	{
		DynamicDroneMaterial->SetVectorParameterValue("TeamColor", NewColor);
	}
}

void ABH_Drone::ResetToPatrol()
{
	SetDroneState(EDroneState::Patrol);
}

// Called every frame
void ABH_Drone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FRotator CurrentRot = GetActorRotation();
	if (GetDroneState() == EDroneState::Patrol)
	{
		if (IsValid(GoalActor))
		{
			GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Blue,FString::Printf(TEXT("Patroling")));
		
			FRotator NextRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GoalActor->GetActorLocation());
			SetActorRotation(UKismetMathLibrary::RInterpTo(CurrentRot, NextRot, DeltaTime,1.0f));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Blue,FString::Printf(TEXT("Chasing Attacker")));
		
			FRotator NextRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetPatrolPoint()->GetActorLocation());
			SetActorRotation(UKismetMathLibrary::RInterpTo(CurrentRot, NextRot, DeltaTime,1.0f));
		}
	}else
	{
		GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Green,FString::Printf(TEXT("Rage")));
		FRotator NextRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GoalActor->GetActorLocation());
		SetActorRotation(UKismetMathLibrary::RInterpTo(CurrentRot, NextRot, DeltaTime,5.0f));
		
	}
	
}

void ABH_Drone::SetupStimulusSource()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	if (StimulusSource)
	{
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->RegisterWithPerceptionSystem();
	}
}

void ABH_Drone::SetDroneState(EDroneState NewState)
{
	CurrentState = NewState;
	if (CurrentState == EDroneState::Rage)
	{
		ChangeDroneColor(RageColor);
		// Additional Rage behaviors
	}
	else
	{
		ChangeDroneColor(CoolColor);
		// Additional Patrol behaviors
	}
}

void ABH_Drone::SetGoalActor(AActor* NewGoal)
{
	GoalActor = NewGoal;
}

