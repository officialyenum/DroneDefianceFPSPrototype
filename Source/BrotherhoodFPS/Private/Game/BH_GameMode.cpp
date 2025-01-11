// Copyright Chukwuyenum Opone @officialyenum 2024


#include "Game/BH_GameMode.h"

#include <functional>

#include "Animation/AnimExecutionContext.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "Player/BH_PlayerController.h"

ABH_GameMode::ABH_GameMode()
{
}

void ABH_GameMode::BeginPlay()
{
	Super::BeginPlay();
	if(ABH_PlayerController* PC = Cast<ABH_PlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		PlayerController = PC;
		PlayerController->Pause();
		ShowWidget(EHUDType::MainMenu);
	}
	if (OnWarmUpTimeChange.IsBound())
	{
		OnWarmUpTimeChange.Broadcast(FormatTimeToText(WarmUpTime));
	}
	if (OnGameTimeChange.IsBound())
	{
		OnGameTimeChange.Broadcast(FormatTimeToText(RemainingTime), FColor::White);
	}
	UpdateGameState(EGameSessionState::MainMenu);
	SetupTutorialList();
	EnablePlayerInput(false);
}

void ABH_GameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ABH_GameMode::TriggerShowTutorial(const FTutorialInfo& TutorialInfo) const
{
	if (OnShowTutorial.IsBound())
	{
		OnShowTutorial.Broadcast(TutorialInfo);
	}
}

FText ABH_GameMode::FormatTimeToText(const float NewTime)
{
	// Calculate minutes and seconds
	int64 FlooredMinute = UKismetMathLibrary::FFloor64(NewTime / 60.f); // Minutes
	int64 FlooredSeconds = UKismetMathLibrary::FFloor64(FMath::Fmod(NewTime, 60.f)); // Seconds

	// Format as MM:SS
	FString TimeString = FString::Printf(TEXT("%02lld:%02lld"), FlooredMinute, FlooredSeconds);
    
	// Convert to FText
	return FText::FromString(TimeString);
}

FTutorialInfo ABH_GameMode::GetTutorialByName(const FName& TutorialName) const
{
	if (!TutorialDT)
	{
		UE_LOG(LogTemp, Error, TEXT("Tutorial Data Table is null!"));
		return FTutorialInfo(); // Return a default-constructed struct to avoid crashes
	}

	// Try to find the row in the data table
	const FTutorialInfo* FoundTutorial = TutorialDT->FindRow<FTutorialInfo>(TutorialName, TEXT("GetTutorialByName"));

	if (!FoundTutorial)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tutorial with name '%s' not found in the data table."), *TutorialName.ToString());
		return FTutorialInfo(); // Return a default-constructed struct if the row is not found
	}
	return *FoundTutorial;
}

void ABH_GameMode::SetupTutorialList()
{
	if (!TutorialDT)
	{
		UE_LOG(LogTemp, Error, TEXT("Tutorial Data Table is null!"));
		return;
	}

	TArray<FTutorialInfo*> TutorialInfoNew;

	// Retrieve all rows from the data table
	TutorialDT->GetAllRows<FTutorialInfo>(TEXT("GetAllTutorialRows"), TutorialInfoNew);

	for (const FTutorialInfo* InfoNew : TutorialInfoNew)
	{
		if (InfoNew == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("InfoNew is nullptr."));
			continue;
		}

		FName NewKey = InfoNew->Title;

		// Check for duplicate keys
		if (TutorialList.Contains(NewKey))
		{
			UE_LOG(LogTemp, Warning, TEXT("Duplicate Key: %s"), *NewKey.ToString());
			continue;
		}

		// Add the tutorial info to the map
		TutorialList.Add(NewKey, InfoNew);
		UE_LOG(LogTemp, Log, TEXT("Added tutorial: %s"), *NewKey.ToString());
	}

}

void ABH_GameMode::StartGame()
{
	RemoveWidget();
	ShowWidget(EHUDType::GameHUD);
	UpdateGameState(EGameSessionState::Live);
	EnablePlayerInput(true);
	UpdateGameTimer();
}

void ABH_GameMode::PauseGame()
{
	EnablePlayerInput(false);
	RemoveWidget();
	ShowWidget(EHUDType::PauseHUD);
	UpdateGameState(EGameSessionState::Paused);
}

void ABH_GameMode::ResumeGame()
{
	RemoveWidget();
	ShowWidget(EHUDType::GameHUD);
	UpdateGameState(EGameSessionState::Live);
	EnablePlayerInput(true);
	UpdateGameTimer();
}

void ABH_GameMode::EnablePlayerInput(bool bValue) const
{
	if (PlayerController)
	{
		if (bValue)
		{
			PlayerController->EnableInput(PlayerController);
		}
		else
		{
			PlayerController->DisableInput(PlayerController);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController is null!"));
	}
}

void ABH_GameMode::UpdateWarmUpTimer()
{
	int NewTime = UKismetMathLibrary::FCeil64(WarmUpTime - UKismetSystemLibrary::GetGameTimeInSeconds(this));
	if (NewTime > 0)
	{
		UpdateGameState(EGameSessionState::WarmUp);
		// Todo:: broadcast Warm Up time to UI
		if (OnWarmUpTimeChange.IsBound())
		{
			OnWarmUpTimeChange.Broadcast(FormatTimeToText(NewTime));
		}
		GetWorld()->GetTimerManager().SetTimer(WarmupTimerHandler, this, &ABH_GameMode::UpdateWarmUpTimer, 1, false);
		return;
	}
	UpdateGameState(EGameSessionState::Live);
	// Todo:: broadcast Game Up time to UI
	
	StartGame();
}

void ABH_GameMode::UpdateGameTimer()
{
	if (PlayerController->IsPaused()) return;
	RemainingTime = FMath::Clamp(RemainingTime - UGameplayStatics::GetWorldDeltaSeconds(this),0, RemainingTime);
	if(RemainingTime <= 15.f)
	{
		if (RemainingTime <= 0.f)
		{
			UpdateGameState(EGameSessionState::OutOfTime);
			return;
		}
		//TODO: Set Text to Red
		if (OnGameTimeChange.IsBound())
		{
			OnGameTimeChange.Broadcast(FormatTimeToText(RemainingTime), FLinearColor::Red);
		}
		return;
	}
	// Todo:: broadcast Game Up time to UI
	if (OnGameTimeChange.IsBound())
	{
		OnGameTimeChange.Broadcast(FormatTimeToText(RemainingTime), FLinearColor::White);
	}
	GetWorld()->GetTimerManager().SetTimer(GameTimerHandler, this, &ABH_GameMode::UpdateGameTimer, 1, false);
}

void ABH_GameMode::UpdateGameState(const EGameSessionState& NewSessionState)
{
	FInputModeGameOnly GameMode;
	// Set input mode to UI only
	FInputModeUIOnly UIMode;
	UIMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // Optional: Decide if you want to lock the mouse
	
	switch (NewSessionState)
	{
		case EGameSessionState::MainMenu:
			PlayerController->bShowMouseCursor = true;
			PlayerController->SetInputMode(UIMode);
			break;
		case EGameSessionState::WarmUp:
			PlayerController->bShowMouseCursor = true;
			PlayerController->SetInputMode(UIMode);
			break;
		case EGameSessionState::Live:
			PlayerController->bShowMouseCursor = false;
			PlayerController->SetInputMode(GameMode);
			break;
		case EGameSessionState::Paused:
			PlayerController->bShowMouseCursor = true;
			PlayerController->SetInputMode(UIMode);
			PlayerController->Pause();
			break;
		case EGameSessionState::OutOfTime:
			PlayerController->bShowMouseCursor = true;
			PlayerController->SetInputMode(UIMode);
			break;
		case EGameSessionState::PlayerDied:
			PlayerController->bShowMouseCursor = true;
			PlayerController->SetInputMode(UIMode);
			break;
		case EGameSessionState::PlayerWon:
			PlayerController->bShowMouseCursor = true;
			PlayerController->SetInputMode(UIMode);
			break;
	}
	SessionState = NewSessionState;
	if (OnStateChange.IsBound())
	{
		OnStateChange.Broadcast(NewSessionState);
	}
}

void ABH_GameMode::ShowWidget(const EHUDType& WidgetType)
{
	CurrentWidget = Cast<UUserWidget>(CreateWidget(PlayerController, *HUDList.Find(WidgetType)));
	CurrentWidget->AddToViewport(1);
}

void ABH_GameMode::RemoveWidget() const
{
	if (CurrentWidget == nullptr) return;
	CurrentWidget->RemoveFromParent();
}

void ABH_GameMode::PawnKilled(APawn* PawnKilled)
{
	
}
