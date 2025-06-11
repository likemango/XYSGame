// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Abilities/XYSGameplayAbility.h"
#include "XYSAbilitySystemComponent.generated.h"

class UXYSGameplayAbility;
class UXYSAbilityTagRelationshipMapping;
/**
 * 
 */
UCLASS()
class XYSGAME_API UXYSAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	/** Sets the current tag relationship mapping, if null it will clear it out */
	void SetTagRelationshipMapping(UXYSAbilityTagRelationshipMapping* NewMapping);
	
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();

	// Uses a gameplay effect to add the specified dynamic granted tag.
	UFUNCTION(BlueprintCallable, Category="XYSAbilitySystem")
	FActiveGameplayEffectHandle AddDynamicTagGameplayEffect(UPARAM(meta=(Categories="CharacterState.Movement")) FGameplayTag Tag);

	//~Begin AbilitySystemComponent
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
	virtual void NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability) override;
	virtual void NotifyAbilityFailed(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason) override;
	virtual void NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, bool bWasCancelled) override;
	virtual void ApplyAbilityBlockAndCancelTags(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags, bool bExecuteCancelTags, const FGameplayTagContainer& CancelTags) override;
	virtual void HandleChangeAbilityCanBeCanceled(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bCanBeCanceled) override;
	//~End

private:
	void TryActivateAbilitiesOnSpawn();
	void AddAbilityToActivationGroup(EXYSAbilityActivationGroup Group, UXYSGameplayAbility* XYSAbility);
	void RemoveAbilityFromActivationGroup(EXYSAbilityActivationGroup Group, UXYSGameplayAbility* XYSAbility);
	
	void CancelActivationGroupAbilities(EXYSAbilityActivationGroup Group, UXYSGameplayAbility* IgnoreXYSAbility, bool bReplicateCancelAbility);
	typedef TFunctionRef<bool(const UXYSGameplayAbility* XYSAbility, FGameplayAbilitySpecHandle Handle)> TShouldCancelAbilityFunc;
	void CancelAbilitiesByFunc(TShouldCancelAbilityFunc ShouldCancelFunc, bool bReplicateCancelAbility);

	/** Notify client that an ability failed to activate */
	UFUNCTION(Client, Unreliable)
	void ClientNotifyAbilityFailed(const UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason);
	void HandleAbilityFailed(const UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason);
	
protected:
	// If set, this table is used to look up tag relationships for activate and cancel
	UPROPERTY()
	TObjectPtr<UXYSAbilityTagRelationshipMapping> TagRelationshipMapping;

	// Handles to abilities that had their input pressed this frame.
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	// Handles to abilities that had their input released this frame.
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	// Handles to abilities that had their input trigger once at a press action.
	// TArray<FGameplayAbilitySpecHandle> InputStartSpecHandles;

	// Handles to abilities that have their input held.
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

	// Number of abilities running in each activation group.
	int32 ActivationGroupCounts[(uint8)EXYSAbilityActivationGroup::MAX];
	
	TMap<FGameplayAbilitySpecHandle, bool> InputStartActivationRecords;
};
