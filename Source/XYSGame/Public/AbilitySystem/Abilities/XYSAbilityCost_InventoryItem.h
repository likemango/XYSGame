// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "XYSAbilityCost.h"
#include "XYSAbilityCost_InventoryItem.generated.h"

class UXYSInventoryItemDefinition;

/**
 * 消耗背包中某个物品（根据等级增加消耗）
 */
UCLASS(meta=(DisplayName="Inventory Item"))
class XYSGAME_API UXYSAbilityCost_InventoryItem : public UXYSAbilityCost
{
	GENERATED_BODY()

public:
	UXYSAbilityCost_InventoryItem();

	//~UXYSAbilityCost interface
	virtual bool CheckCost(const UXYSGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ApplyCost(const UXYSGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	//~End of UXYSAbilityCost interface

protected:
	/** How much of the item to spend (keyed on ability level) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=AbilityCost)
	FScalableFloat Quantity;

	/** Which item to consume */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=AbilityCost)
	TSubclassOf<UXYSInventoryItemDefinition> ItemDefinition;
};
