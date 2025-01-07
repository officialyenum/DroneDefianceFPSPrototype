// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Pawn/BH_Drone.h"

#include "NavigationSystem.h"
#include "AI/Controller/BH_DroneAiController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/BH_CharacterPlayer.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"


// Sets default values
ABH_Drone::ABH_Drone()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AAIController::StaticClass();
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComponent);
	PawnSense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sense"));
	PawnSense->SensingInterval = .8f;
	PawnSense->SetPeripheralVisionAngle(45.f);
	PawnSense->SightRadius = 1500.f;
	PawnSense->HearingThreshold = 400.f;
	PawnSense->LOSHearingThreshold = 800.F;

	Coll =  CreateDefaultSubobject<USphereComponent>("ExplosionSphere");
	GetCollision()->SetSphereRadius(100.f);
	GetCollision()->SetupAttachment(RootComponent);
	GetCollision()->SetGenerateOverlapEvents(true);
	
	DroneMesh = CreateDefaultSubobject<USkeletalMeshComponent>("DroneMesh");
	DroneMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
	SkeletalMeshAsset(TEXT("/Game/Assets/Characters/ParagonLtBelica/Characters/Heroes/Belica/Meshes/Belica_Drone/Meshes/Belica_Drone"));
	if(SkeletalMeshAsset.Succeeded())
	{
		DroneMesh->SetSkeletalMesh(SkeletalMeshAsset.Object);
	}
	
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("FloatingPawnMovement");

	FloatingPawnMovement->MaxSpeed = PatrolSpeed;
	// Default Values
}

// Called when the game starts or when spawned
void ABH_Drone::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetActorLocation();
	DynamicDroneMaterial = DroneMesh->CreateDynamicMaterialInstance(0,DroneMesh->GetMaterial(0));
	DroneMesh->SetMaterial(0,DynamicDroneMaterial);
	SetNextPatrolLocation();
}

void ABH_Drone::ExplosionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,FString::Printf(TEXT("Explosion Sphere Overlapped ")));
	if (ABH_CharacterPlayer* Player = Cast<ABH_CharacterPlayer>(OtherActor))
	{
		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Green,FString::Printf(TEXT("Player Overlapped Explosion Sphere")));
		TSubclassOf<UDamageType> DamageType;
		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Blue,FString::Printf(TEXT("Damage Applied To BP_Player")));
		UGameplayStatics::ApplyDamage(Player,BulletDamage,GetController(),this,DamageType);
		Explode();
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
		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,FString::Printf(TEXT("Drone Took Damage")));
	}
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

// Called every frame
void ABH_Drone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FloatingPawnMovement->GetMaxSpeed() == ChaseSpeed) return;
	if ((GetActorLocation() - PatrolLocation).Size() < 500.f)
	{
		SetNextPatrolLocation();
	}
}

void ABH_Drone::SetNextPatrolLocation()
{
	FloatingPawnMovement->MaxSpeed = PatrolSpeed;
	ChangeDroneColor(CoolColor);
	PatrolCounter--;
	if (PatrolCounter <= 0)
	{
		PatrolCounter = 2;
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), StartLocation);
	}
	if (const auto LocationFound = UNavigationSystemV1::K2_GetRandomReachablePointInRadius(this, GetActorLocation(), PatrolLocation, PatrolRadius))
	{
		
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), PatrolLocation);
	}
}

void ABH_Drone::Chase(APawn* Pawn)
{
	FloatingPawnMovement->MaxSpeed = ChaseSpeed;
	UE_LOG(LogTemp, Display, TEXT("Chasing Player Speed %f "), FloatingPawnMovement->MaxSpeed);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Chasing Player Speed"));
	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), Pawn);
	ChangeDroneColor(RageColor);

	DrawDebugSphere(GetWorld(), Pawn->GetActorLocation(), 25.f, 12, FColor::Red, true, 10.f, 0, 2.0f);
}

void ABH_Drone::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GetCollision()->OnComponentBeginOverlap.AddDynamic(this,&ABH_Drone::ExplosionSphereBeginOverlap);
	GetPawnSense()->OnSeePawn.AddDynamic(this, &ABH_Drone::OnPawnDetected);
	OnTakeAnyDamage.AddDynamic(this,&ABH_Drone::TakeHitDamage);
	ChangeDroneColor(CoolColor);
}

void ABH_Drone::OnPawnDetected(APawn* Pawn)
{
	if(!Pawn->IsA<ABH_CharacterPlayer>()) return;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Character detected"));

	if(FloatingPawnMovement->GetMaxSpeed() != ChaseSpeed)
	{
		Chase(Pawn);
	}
}

