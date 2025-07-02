// XiaoYao copyright.


#include "AbilitySystem/Abilities/XYSAbilityCost_InventoryItem.h"

#include "AbilitySystem/Abilities/XYSGameplayAbility.h"
#include "Inventory/XYSInventoryComponent.h"

UXYSAbilityCost_InventoryItem::UXYSAbilityCost_InventoryItem()
{
	Quantity.SetValue(1.0f);
}

bool UXYSAbilityCost_InventoryItem::CheckCost(const UXYSGameplayAbility* Ability,
	const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	if (AController* PC = Ability->GetControllerFromActorInfo())
	{
		if (UXYSInventoryComponent* InventoryComponent = PC->GetComponentByClass<UXYSInventoryComponent>())
		{
			const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

			const float NumItemsToConsumeReal = Quantity.GetValueAtLevel(AbilityLevel);
			const int32 NumItemsToConsume = FMath::TruncToInt(NumItemsToConsumeReal);

			return InventoryComponent->GetTotalItemCountByDefinition(ItemDefinition) >= NumItemsToConsume;
		}
	}
	return false;
}

void UXYSAbilityCost_InventoryItem::ApplyCost(const UXYSGameplayAbility* Ability,
	const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo->IsNetAuthority())
	{
		if (AController* PC = Ability->GetControllerFromActorInfo())
		{
			if (UXYSInventoryComponent* InventoryComponent = PC->GetComponentByClass<UXYSInventoryComponent>())
			{
				const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

				const float NumItemsToConsumeReal = Quantity.GetValueAtLevel(AbilityLevel);
				const int32 NumItemsToConsume = FMath::TruncToInt(NumItemsToConsumeReal);

				InventoryComponent->ConsumeItemsByDefinition(ItemDefinition, NumItemsToConsume);
			}
		}
	}
}
