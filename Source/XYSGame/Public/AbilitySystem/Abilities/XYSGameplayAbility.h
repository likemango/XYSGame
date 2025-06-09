// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "XYSGameplayAbility.generated.h"

class AXYSCharacter;
class UXYSCharacterMovementComponent;
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

	virtual void OnPawnAvatarSet();
	/** Called when the ability system is initialized with a pawn avatar. */
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnPawnAvatarSet")
	void K2_OnPawnAvatarSet();

	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;
	
	EXYSAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

	UFUNCTION(BlueprintCallable, Category = "Ability")
	AXYSCharacter* GetXYSCharacterFromActorInfo() const;
	UFUNCTION(BlueprintCallable, Category = "Ability")
	UXYSCharacterMovementComponent* GetXYSCharacterMovementFromActorInfo() const;
	
protected:
	// Defines how this ability is meant to activate.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Activation")
	EXYSAbilityActivationPolicy ActivationPolicy;
};
