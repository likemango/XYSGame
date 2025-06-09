// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "XYSAbilitySystemComponent.generated.h"

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

	//~Begin AbilitySystemComponent
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
	//~End

private:
	void TryActivateAbilitiesOnSpawn();
	
	
protected:
	// If set, this table is used to look up tag relationships for activate and cancel
	UPROPERTY()
	TObjectPtr<UXYSAbilityTagRelationshipMapping> TagRelationshipMapping;

	// Handles to abilities that had their input pressed this frame.
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	// Handles to abilities that had their input released this frame.
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	// Handles to abilities that have their input held.
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
};
