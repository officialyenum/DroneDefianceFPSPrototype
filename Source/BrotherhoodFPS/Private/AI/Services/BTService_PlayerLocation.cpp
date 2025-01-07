// Copyright Chukwuyenum Opone @officialyenum 2024


#include "AI/Services/BTService_PlayerLocation.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTService_PlayerLocation::UBTService_PlayerLocation()
{
	NodeName = "Update Player Location";
}

void UBTService_PlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn == nullptr)
	{
		return;
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), PlayerPawn);
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(LastKnownLocation.SelectedKeyName, PlayerPawn->GetActorLocation());
}
