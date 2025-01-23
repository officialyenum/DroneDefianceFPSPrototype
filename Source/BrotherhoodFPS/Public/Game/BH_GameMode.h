// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BH_GameMode.generated.h"

// Forward declarations
class ABH_PlayerController;
class UDataTable;
class UUserWidget;
struct FTutorialInfo;

// Enumeration for game session states
UENUM(BlueprintType)
enum class EGameSessionState : uint8
{
	MainMenu     UMETA(DisplayName = "Main Menu"),
	WarmUp       UMETA(DisplayName = "Warm Up"),
	Live         UMETA(DisplayName = "Live"),
	Paused       UMETA(DisplayName = "Paused"),
	OutOfTime    UMETA(DisplayName = "Out Of Time"),
	PlayerDied   UMETA(DisplayName = "Player Died"),
	PlayerWon    UMETA(DisplayName = "Player Won")
};

UENUM(BlueprintType)
enum class EHUDType : uint8
{
	MainMenu     UMETA(DisplayName = "Main Menu"),
	WarmUpHUD      UMETA(DisplayName = "Warm Up HUD"),
	GameHUD      UMETA(DisplayName = "Game HUD"),
	PauseHUD     UMETA(DisplayName = "Pause HUD"),
	OutOfTimeHUD UMETA(DisplayName = "Out Of Time HUD"),
	PlayerDiedHUD UMETA(DisplayName = "Player Died HUD"),
	PlayerWonHUD UMETA(DisplayName = "Player Won HUD"),
	SettingsHUD  UMETA(DisplayName = "Settings HUD")
};

UENUM(BlueprintType)
enum class EInputModifierType : uint8
{
	Press UMETA(DisplayName = "Press"),
	Hold UMETA(DisplayName = "Hold"),
	Move UMETA(DisplayName = "Move"),
};

USTRUCT(BlueprintType)
struct FQuest : public  FTableRowBase
{
	GENERATED_BODY()

public:
	// Name of the quest
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FName QuestName;

	// Description of the quest
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FString Description;

	// Is the quest completed?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	bool bIsCompleted;

	FQuest()
		: QuestName(NAME_None), Description(TEXT("")), bIsCompleted(false)
	{
	}
};

USTRUCT(BlueprintType)
struct FTutorialInfo : public  FTableRowBase
{
	GENERATED_BODY()
public:
	FTutorialInfo(): Type(EInputModifierType::Press), Icon(nullptr)
	{
	}

	/** Name */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tutorial",meta=(AllowPrivateAccess = "true"))
	FName Title;
	/** Instructions */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tutorial",meta=(AllowPrivateAccess = "true"))
	FName Instructions;
	/** Type */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tutorial",meta=(AllowPrivateAccess = "true"))
	EInputModifierType Type;
	/** Icon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tutorial",meta=(AllowPrivateAccess = "true"))
	UTexture2D* Icon;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShowTutorialSignature, const FTutorialInfo&, TutorialInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateChangeSignature, EGameSessionState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWarmUpTimeChangeSignature, FText, FormattedTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameTimeChangeSignature, FText, FormattedTime, FLinearColor, TimeColor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDronesRemainingSignature, FText, DronesRemainingText);


UCLASS()
class BROTHERHOODFPS_API ABH_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ABH_GameMode();
	
	// Game Mode overrides
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	// State management functions
	UFUNCTION(BlueprintCallable, Category = "GM|State")
	void StartWarmUp();
	UFUNCTION(BlueprintCallable, Category = "GM|State")
	void StartGame();
	UFUNCTION(BlueprintCallable, Category = "GM|State")
	void PauseGame();
	UFUNCTION(BlueprintCallable, Category = "GM|State")
	void ResumeGame();
	UFUNCTION(BlueprintCallable, Category = "GM|State")
	void UpdateWarmUpTimer();
	UFUNCTION(BlueprintCallable, Category = "GM|State")
	void UpdateGameTimer();
	UFUNCTION(BlueprintCallable, Category = "GM|State")
	void UpdateGameState(const EGameSessionState& NewState);

	// Helper functions
	UFUNCTION(BlueprintCallable, Category = "GM|Helper|Functions")
	virtual void PawnKilled(APawn* PawnKilled);
	UFUNCTION(BlueprintCallable, Category = "GM|Helper|Functions")
	FText FormatTimeToText(float TimeInSeconds) const;
	UFUNCTION(BlueprintCallable, Category = "GM|Helper|Functions")
	bool AreAllQuestsCompleted() const;
	UFUNCTION(BlueprintCallable, Category = "GM|Helper|Functions")
	void UpdateMode(bool bIsUI) const;
protected:
	// UI handling
	UFUNCTION(BlueprintCallable, Category = "GM|Functions")
	void ShowWidget(const EHUDType& WidgetType);
	UFUNCTION(BlueprintCallable, Category = "GM|Functions")
	void RemoveWidget() const;

	// Player input management
	UFUNCTION(BlueprintCallable, Category = "GM|Functions")
	void EnablePlayerInput(bool bEnable) const;

	// Tutorial management
	UFUNCTION(BlueprintCallable, Category = "GM|Functions")
	void SetupTutorialList();
	FTutorialInfo GetTutorialByName(const FName& TutorialName) const;

	// Timer-related broadcasting
	UFUNCTION(BlueprintCallable, Category = "GM|Functions")
	void BroadcastWarmUpTime(int32 Time) const;
	UFUNCTION(BlueprintCallable, Category = "GM|Functions")
	void BroadcastRemainingTime(int32 Time) const;
private:
	// State tracking
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GM|State", meta = (AllowPrivateAccess = "true"))
	EGameSessionState SessionState;

	// Player controller reference
	UPROPERTY()
	ABH_PlayerController* PlayerController;

	// HUD management
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GM|UI", meta = (AllowPrivateAccess = "true"))
	TMap<EHUDType, TSubclassOf<UUserWidget>> HUDList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GM|UI", meta = (AllowPrivateAccess = "true"))
	UUserWidget* CurrentWidget;

	// Timing variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GM|Game Time", meta = (AllowPrivateAccess = "true"))
	float WarmUpTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GM|Game Time", meta = (AllowPrivateAccess = "true"))
	float RemainingTime;

	// Drone tracking
	UPROPERTY(BlueprintReadOnly, Category = "GM|Drones", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> DronesLeft;

	// Tutorials
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GM|Tutorials", meta = (AllowPrivateAccess = "true"))
	UDataTable* TutorialDT;

	UPROPERTY(BlueprintReadOnly, Category = "GM|Tutorials", meta = (AllowPrivateAccess = "true"))
	TMap<FName, FTutorialInfo> TutorialList;

	// Quests List
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GM|Quest", meta = (AllowPrivateAccess = "true"))
	TArray<FQuest> QuestList;

	// Timer handles
	FTimerHandle WarmupTimerHandler;
	FTimerHandle GameTimerHandler;
	
	// Events
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "GM|Events")
	FOnShowTutorialSignature OnShowTutorial;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "GM|Events")
	FOnStateChangeSignature OnStateChange;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "GM|Events")
	FOnWarmUpTimeChangeSignature OnWarmUpTimeChange;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "GM|Events")
	FOnGameTimeChangeSignature OnGameTimeChange;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "GM|Events")
	FOnDronesRemainingSignature OnDronesRemainingSignature;
};
