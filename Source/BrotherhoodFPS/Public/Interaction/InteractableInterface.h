// Copyright Chukwuyenum Opone @officialyenum 2024

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BROTHERHOODFPS_API IInteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interaction", meta=(DisplayName="Interact"))
	void Interact(class ABH_CharacterBase* CharacterInstigator);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interaction", meta=(DisplayName="Interact"))
	bool CanInteract(class ABH_CharacterBase* CharacterInstigator) const;
};
