// Copyright Vinipi Studios 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InteractionKey.generated.h"

class UInputAction;

/**
 * Used to define info about the interaction input.
 */
UCLASS(BlueprintType)
class CUSTOMIZABLEINTERACTIONPLUGIN_API UInteractionKey : public UObject
{
	GENERATED_BODY()
	
public:
	UInteractionKey();
	
	UPROPERTY(EditAnywhere, Category = "Interaction Key")
	bool bConsumeInputAfterInteraction = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction Key")
	TObjectPtr<UInputAction> InputAction;

	UFUNCTION(BlueprintGetter, Category = "Interaction Key")
	UInputAction* GetInputAction() const { return InputAction; }
};
