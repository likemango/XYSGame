// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "XYSAbilityCost.h"
#include "XYSAbilityCost_PlayerTagStack.generated.h"

/**
 * 消耗playerState上的TagStack
 */
UCLASS(meta=(DisplayName="Player Tag Stack"))
class XYSGAME_API UXYSAbilityCost_PlayerTagStack : public UXYSAbilityCost
{
	GENERATED_BODY()

public:
	UXYSAbilityCost_PlayerTagStack();

	//~UXYSAbilityCost interface
	virtual bool CheckCost(const UXYSGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ApplyCost(const UXYSGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	//~End of UXYSAbilityCost interface

protected:
	/** How much of the tag to spend (keyed on ability level) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Costs)
	FScalableFloat Quantity;

	/** Which tag to spend some of */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Costs)
	FGameplayTag Tag;
};
