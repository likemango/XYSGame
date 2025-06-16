// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "XYSGameplayAbility.generated.h"

class UXYSHeroComponent;
class UXYSAbilitySystemComponent;
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

	// Only trigger once for a press/release button
	OnInputStarted,

	// Continually try to activate the ability while the input is active.
	WhileInputActive,

	// Try to activate the ability when an avatar is assigned.
	OnSpawn
};

/**
 * EXYSAbilityActivationGroup
 *
 *	Defines how an ability activates in relation to other abilities.
 */
UENUM(BlueprintType)
enum class EXYSAbilityActivationGroup : uint8
{
	// Ability runs independently of all other abilities.
	Independent,

	// Ability is canceled and replaced by other exclusive abilities.
	Exclusive_Replaceable,

	// Ability blocks all other exclusive abilities from activating.
	Exclusive_Blocking,

	MAX	UMETA(Hidden)
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
	EXYSAbilityActivationGroup GetActivationGroup() const { return ActivationGroup; }

	UFUNCTION(BlueprintCallable, Category = "Ability")
	AXYSCharacter* GetXYSCharacterFromActorInfo() const;
	UFUNCTION(BlueprintCallable, Category = "Ability")
	UXYSCharacterMovementComponent* GetXYSCharacterMovementFromActorInfo() const;
	UFUNCTION(BlueprintCallable, Category = "Ability")
	UXYSAbilitySystemComponent* GetXYSAbilitySystemComponentFromActorInfo() const;
	UFUNCTION(BlueprintCallable, Category = "Ability")
	AController* GetControllerFromActorInfo() const;
	UFUNCTION(BlueprintCallable, Category = "Ability")
	UXYSHeroComponent* GetHeroComponentFromActorInfo() const;

	void OnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const
	{
		NativeOnAbilityFailedToActivate(FailedReason);
		K2_OnAbilityFailedToActivate(FailedReason);
	}

protected:
	// Called when the ability fails to activate(Native)
	virtual void NativeOnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const;
	// Called when the ability fails to activate(Blueprint)
	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const;
	
protected:
	// Defines how this ability is meant to activate.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Activation")
	EXYSAbilityActivationPolicy ActivationPolicy;
	// Defines the relationship between this ability activating and other abilities activating.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Activation")
	EXYSAbilityActivationGroup ActivationGroup;
};
