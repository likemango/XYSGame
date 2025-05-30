// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "XYSGameplayAbility.generated.h"

/**
 * EXYSAbilityActivationPolicy
 *
 *	Defines how an ability is meant to activate.
 */
UENUM(BlueprintType)
enum class EXYSAbilityActivationPolicy : uint8
{
	// Try to activate the ability when the input is triggered.
	OnInputTriggered,

	// Continually try to activate the ability while the input is active.
	WhileInputActive,

	// Try to activate the ability when an avatar is assigned.
	OnSpawn
};

/**
 * 
 */
UCLASS()
class XYSGAME_API UXYSGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UXYSGameplayAbility(const FObjectInitializer& ObjectInitializer);

	EXYSAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

protected:
	// Defines how this ability is meant to activate.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Activation")
	EXYSAbilityActivationPolicy ActivationPolicy;
};
