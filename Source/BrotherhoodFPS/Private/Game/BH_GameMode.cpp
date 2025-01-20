// Copyright Chukwuyenum Opone @officialyenum 2024

#include "Game/BH_GameMode.h"

#include "Animation/AnimExecutionContext.h"
#include "Blueprint/UserWidget.h"
#include "Character/BH_DroneSandBox.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "Player/BH_PlayerController.h"

ABH_GameMode::ABH_GameMode()
    : SessionState(EGameSessionState::MainMenu),
      PlayerController(nullptr), // Default warm-up time
      CurrentWidget(nullptr), WarmUpTime(3),
      // Default remaining time
      RemainingTime(300),
      TutorialDT(nullptr)
{
}

void ABH_GameMode::BeginPlay()
{
    Super::BeginPlay();

    // Retrieve the player controller
    PlayerController = Cast<ABH_PlayerController>(GetWorld()->GetFirstPlayerController());
    if (PlayerController)
    {
        PlayerController->SetPause(true);
        // PlayerController->Pause();
    }
    // Initialize timers and state
    UpdateGameState(EGameSessionState::MainMenu);
    SetupTutorialList();
    UpdateMode(true);

    // override the first widget if the default is main menu
    RemoveWidget();
    ShowWidget(EHUDType::MainMenu);
    EnablePlayerInput(false);
}

void ABH_GameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    // Game-specific tick functionality can be added here if necessary
}

void ABH_GameMode::StartWarmUp()
{
    // Transition to Warm Up state
    PlayerController->SetPause(false);
    UpdateGameState(EGameSessionState::WarmUp);
    DronesLeft.Empty();
    UGameplayStatics::GetAllActorsOfClass(this, ABH_DroneSandBox::StaticClass(), DronesLeft);
    GetWorld()->GetTimerManager().SetTimer(WarmupTimerHandler, this, &ABH_GameMode::UpdateWarmUpTimer, 1.f, true, 1.f);
}

void ABH_GameMode::StartGame()
{
    // Transition to Live state
    UpdateGameState(EGameSessionState::Live);
    GetWorld()->GetTimerManager().SetTimer(GameTimerHandler, this, &ABH_GameMode::UpdateGameTimer, 1.f, true, 1.f);
}

void ABH_GameMode::PauseGame()
{
    if (SessionState == EGameSessionState::Live)
    {
        UpdateGameState(EGameSessionState::Paused);
        GetWorld()->GetTimerManager().PauseTimer(GameTimerHandler);
    }
}

void ABH_GameMode::ResumeGame()
{
    if (SessionState == EGameSessionState::Paused)
    {
        UpdateGameState(EGameSessionState::Live);
        GetWorld()->GetTimerManager().UnPauseTimer(GameTimerHandler);
    }
}

void ABH_GameMode::UpdateWarmUpTimer()
{
    int32 NewTime = FMath::CeilToInt(WarmUpTime - UKismetSystemLibrary::GetGameTimeInSeconds(this));
    if (NewTime > -1)
    {
        BroadcastWarmUpTime(NewTime);
    }
    else
    {
        GetWorld()->GetTimerManager().ClearTimer(WarmupTimerHandler);
        StartGame();
    }
}

void ABH_GameMode::UpdateGameTimer()
{
    if (SessionState == EGameSessionState::Live)
    {
        int32 NewTime = FMath::CeilToInt(RemainingTime - UKismetSystemLibrary::GetGameTimeInSeconds(this));
        if (NewTime > 0)
        {
            BroadcastRemainingTime(NewTime);
        }
        else
        {
            UpdateGameState(EGameSessionState::OutOfTime);
            GetWorld()->GetTimerManager().PauseTimer(GameTimerHandler);
        }
    }
}

void ABH_GameMode::PawnKilled(APawn* PawnKilled)
{
    if (SessionState == EGameSessionState::Live)
    {
        if (AActor* Drone = Cast<AActor>(PawnKilled))
        {
            if (DronesLeft.Contains(Drone))
            {
                DronesLeft.Remove(Drone);
            }

            // Update remaining drones
            FString DroneText = DronesLeft.Num() > 1 ? FString::Printf(TEXT("Drones Remaining: %d"), DronesLeft.Num())
                                                     : FString::Printf(TEXT("Drone Remaining: %d"), DronesLeft.Num());
            OnDronesRemainingSignature.Broadcast(FText::FromString(DroneText));

            if (DronesLeft.Num() == 0 && AreAllQuestsCompleted())
            {
                UpdateGameState(EGameSessionState::PlayerWon);
            }
        }
    }
}

void ABH_GameMode::UpdateGameState(const EGameSessionState& NewState)
{
    // Prevent redundant state updates
    if (SessionState == NewState) return;

    SessionState = NewState;
    RemoveWidget();
    UpdateMode(true);
    // Handle state-specific logic
    switch (NewState)
    {
    case EGameSessionState::MainMenu:
        ShowWidget(EHUDType::MainMenu);
        EnablePlayerInput(false);
        break;
    case EGameSessionState::WarmUp:
        ShowWidget(EHUDType::WarmUpHUD);
        EnablePlayerInput(false);
        break;
    case EGameSessionState::Live:
        PlayerController->SetPause(false);
        UpdateMode(false);
        ShowWidget(EHUDType::GameHUD);
        EnablePlayerInput(true);
        break;
    case EGameSessionState::Paused:
        PlayerController->SetPause(true);
        ShowWidget(EHUDType::PauseHUD);
        EnablePlayerInput(false);
        break;
    case EGameSessionState::OutOfTime:
        ShowWidget(EHUDType::OutOfTimeHUD);
        EnablePlayerInput(false);
        break;
    case EGameSessionState::PlayerDied:
        ShowWidget(EHUDType::PlayerDiedHUD);
        EnablePlayerInput(false);
        break;
    case EGameSessionState::PlayerWon:
        ShowWidget(EHUDType::PlayerWonHUD);
        EnablePlayerInput(false);
        break;
    }
    // Notify other systems about the state change
    if (OnStateChange.IsBound())
    {
        OnStateChange.Broadcast(NewState);
    }
}

bool ABH_GameMode::AreAllQuestsCompleted() const
{
    // Check if all quests are completed
    return QuestList.Num() > 0 && QuestList.ContainsByPredicate([](const FQuest& Quest) { return !Quest.bIsCompleted; }) == false;
}

void ABH_GameMode::UpdateMode(bool bIsUI) const
{
    FInputModeGameOnly GameMode;
    // Set input mode to UI only
    if (bIsUI)
    {
        FInputModeUIOnly UIMode;
        UIMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // Optional: Decide if you want to lock the mouse
        PlayerController->bShowMouseCursor = true;
        PlayerController->SetInputMode(UIMode);
        return;
    }
    PlayerController->bShowMouseCursor = false;
    PlayerController->SetInputMode(GameMode);
    
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

void ABH_GameMode::EnablePlayerInput(bool bEnable) const
{
    if (PlayerController)
    {
        if (bEnable)
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
        TutorialList.Add(NewKey, *InfoNew);
        UE_LOG(LogTemp, Log, TEXT("Added tutorial: %s"), *NewKey.ToString());
    }
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

void ABH_GameMode::BroadcastWarmUpTime(int32 Time) const
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f,FColor::Red,FString::Printf(TEXT("Warm Up Time ... %i"), Time));
    if (OnWarmUpTimeChange.IsBound())
    {
        FText TimeText = FText::FromString(FString::Printf(TEXT("%i"), Time));
        if (Time <= 0)
        {
            TimeText = FText::FromString(FString::Printf(TEXT("Get Ready...")));
        }
        OnWarmUpTimeChange.Broadcast(TimeText);
    }
}

void ABH_GameMode::BroadcastRemainingTime(int32 Time) const
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f,FColor::Red,FString::Printf(TEXT("Remaining Time... %i"), Time));
    if (OnGameTimeChange.IsBound())
    {
        FLinearColor Color = Time <= 15.f ? FLinearColor::Red : FLinearColor::White;
        OnGameTimeChange.Broadcast(FormatTimeToText(Time), Color);
    }
}

FText ABH_GameMode::FormatTimeToText(float TimeInSeconds) const
{
    int32 Minutes = FMath::FloorToInt(TimeInSeconds / 60.f);
    int32 AltSeconds = FMath::FloorToInt(TimeInSeconds - (Minutes * 60.f));
    int32 Seconds = FMath::FloorToInt(FMath::Fmod(TimeInSeconds, 60.f));
    GEngine->AddOnScreenDebugMessage(-1, 3.f,FColor::Blue,FString::Printf(TEXT("Formatted Time Alt : %02d:%02d mm:ss"), Minutes, AltSeconds));
    GEngine->AddOnScreenDebugMessage(-1, 3.f,FColor::Blue,FString::Printf(TEXT("Formatted Time : %02d:%02d mm:ss"), Minutes, Seconds));
    return FText::FromString(FString::Printf(TEXT("Remaining Time : %02d:%02d mm:ss"), Minutes, AltSeconds));
}
