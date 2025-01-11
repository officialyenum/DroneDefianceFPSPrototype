// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BH_GameMode.generated.h"




class ABH_PlayerController;
/**
 * 
 */
UENUM(BlueprintType)
enum class EGameSessionState : uint8
{
	MainMenu UMETA(DisplayName = "MainMenu"),
	WarmUp UMETA(DisplayName = "WarmUp"),
	Live UMETA(DisplayName = "Live"),
	Paused UMETA(DisplayName = "Paused"),
	OutOfTime UMETA(DisplayName = "Out Of Time"),
	PlayerDied UMETA(DisplayName = "Player Died"),
	PlayerWon UMETA(DisplayName = "Player Won"),
};

UENUM(BlueprintType)
enum class EHUDType : uint8
{
	MainMenu UMETA(DisplayName = "MainMenu"),
	CreditHUD UMETA(DisplayName = "CreditHUD"),
	SettingsHUD UMETA(DisplayName = "SettingsHUD"),
	GameHUD UMETA(DisplayName = "GameHUD"),
	PauseHUD UMETA(DisplayName = "PauseHUD"),
	EndGameHUD UMETA(DisplayName = "EndGameHUD"),
};

UENUM(BlueprintType)
enum class EInputModifierType : uint8
{
	Press UMETA(DisplayName = "Press"),
	Hold UMETA(DisplayName = "Hold"),
	Move UMETA(DisplayName = "Move"),
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShowTutorialSignature, const FTutorialInfo&, TutorialInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStateChangeSignature, const EGameSessionState&, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGameTimeChangeSignature, const FText&, newGameTime, const FLinearColor&, NewColor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWarmUpTimeChangeSignature, const FText&, newWarmUpTime);


UCLASS()
class BROTHERHOODFPS_API ABH_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ABH_GameMode();
	
	virtual void PawnKilled(APawn* PawnKilled);
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintAssignable, Category = "GM|Events")
	FShowTutorialSignature OnShowTutorial;
	UPROPERTY(BlueprintAssignable, Category = "GM|Events")
	FStateChangeSignature OnStateChange;
	UPROPERTY(BlueprintAssignable, Category = "GM|Events")
	FGameTimeChangeSignature OnGameTimeChange;
	UPROPERTY(BlueprintAssignable, Category = "GM|Events")
	FWarmUpTimeChangeSignature OnWarmUpTimeChange;
	
	void TriggerShowTutorial(const FTutorialInfo& TutorialInfo) const;
	static FText FormatTimeToText(const float NewTime);
	
	UFUNCTION(BlueprintCallable, Category = "GM|Functions")
	void StartGame();
	
	UFUNCTION(BlueprintCallable, Category = "GM|Functions")
	void PauseGame();
	
	UFUNCTION(BlueprintCallable, Category = "GM|Functions")
	void ResumeGame();
	
protected:
	UFUNCTION(BlueprintCallable)
	FTutorialInfo GetTutorialByName(const FName& TutorialName) const;
	
	UFUNCTION()
	void SetupTutorialList();
	
	UFUNCTION()
	void EnablePlayerInput(bool bValue) const;

	UFUNCTION()
	void UpdateWarmUpTimer();
	
	UFUNCTION()
	void UpdateGameTimer();

	UFUNCTION()
	void UpdateGameState(const EGameSessionState& NewSessionState);
	
	UFUNCTION()
	void ShowWidget(const EHUDType& WidgetType);
	
	UFUNCTION()
	void RemoveWidget() const;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GM|Param",meta=(AllowPrivateAccess = "true"))
	EGameSessionState SessionState;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GM|Param",meta=(AllowPrivateAccess = "true"))
	TMap<EHUDType, TSubclassOf<class UUserWidget>> HUDList;
	TMap<FName, const FTutorialInfo*> TutorialList;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GM|Param",meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> TutorialDT;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GM|Param",meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UUserWidget> CurrentWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GM|Param",meta=(AllowPrivateAccess = "true"))
	float WarmUpTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GM|Param",meta=(AllowPrivateAccess = "true"))
	float RemainingTime;

	FTimerHandle WarmupTimerHandler;
	FTimerHandle GameTimerHandler;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GM|Param",meta=(AllowPrivateAccess = "true"))
	TMap<FString, bool> QuestList;
	ABH_PlayerController* PlayerController;
};
