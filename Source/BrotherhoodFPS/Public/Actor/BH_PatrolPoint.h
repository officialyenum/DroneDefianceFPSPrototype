// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BH_PatrolPoint.generated.h"

UCLASS()
class BROTHERHOODFPS_API ABH_PatrolPoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABH_PatrolPoint();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Next Location")
	ABH_PatrolPoint* NextLocation;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
