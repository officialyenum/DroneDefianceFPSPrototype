// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Actor/BH_Gun.h"

#include "Character/BH_CharacterBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABH_Gun::ABH_Gun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>("DefaultScene");
	GunComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Gun");
	GunComponent->SetupAttachment(RootComponent);
	GunComponent->SetGenerateOverlapEvents(false);
	GunComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
	//Gun->AttachToComponent(GetMesh(), Rules, FName("GripPoint"));
	
	BurstPoint = CreateDefaultSubobject<USceneComponent>("BurstPoint");
	BurstPoint->SetupAttachment(GunComponent);

}

// Called when the game starts or when spawned
void ABH_Gun::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABH_Gun::PlaySoundAndBurstEmitterFX()
{
	FVector Loc = BurstPoint->GetComponentLocation();
	FRotator Rot = BurstPoint->GetComponentRotation();
	FVector Scale = FVector(0.25);
	UGameplayStatics::SpawnEmitterAtLocation(this, ShotBurstFX, Loc, Rot, Scale, true);
	UGameplayStatics::PlaySoundAtLocation(this, ShotSound, Loc, Rot, 0.5f);
}

void ABH_Gun::ResetCanShoot()
{
	CanShoot = true;
}

AActor* ABH_Gun::PerformLineTrace()
{
	AActor* Camera = UGameplayStatics::GetPlayerCameraManager(this,0);
	FVector Loc = Camera->GetActorLocation();
	FVector FV = Camera->GetActorForwardVector() * 5000;
	FVector LOCFV = Loc + FV;

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Loc, LOCFV, ECC_Visibility);
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactFX, HitResult.ImpactPoint,FRotator(0.F),FVector(1));
	if (ICombatInterface* Actor = Cast<ICombatInterface>(HitResult.GetActor()))
	{
		return HitResult.GetActor();
	}
	return nullptr;
}

void ABH_Gun::Reload()
{
	if (Cartridge > 0)
	{
		Cartridge--;
		Ammo = FMath::Clamp(MaxAmmo, 0, MaxAmmo);
		UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetActorLocation(), GetActorRotation(), 0.5f);
	}
}

void ABH_Gun::AddCartridge(int32 NewCartRidge)
{
	Cartridge = FMath::Clamp(Cartridge + NewCartRidge, 0, MaxCartridge);
}



