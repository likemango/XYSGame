// XiaoYao copyright.


#include "AbilitySystem/Abilities/XYSAbilityCost_PlayerTagStack.h"

#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystem/Abilities/XYSGameplayAbility.h"
#include "Player/XYSPlayerState.h"

UXYSAbilityCost_PlayerTagStack::UXYSAbilityCost_PlayerTagStack()
{
	Quantity.SetValue(1.0f);
}

bool UXYSAbilityCost_PlayerTagStack::CheckCost(const UXYSGameplayAbility* Ability,
	const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	if (AController* PC = Ability->GetControllerFromActorInfo())
	{
		if (AXYSPlayerState* PS = Cast<AXYSPlayerState>(PC->PlayerState))
		{
			const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

			const float NumStacksReal = Quantity.GetValueAtLevel(AbilityLevel);
			const int32 NumStacks = FMath::TruncToInt(NumStacksReal);

			return PS->GetStatTagStackCount(Tag) >= NumStacks;
		}
	}
	return false;
}

void UXYSAbilityCost_PlayerTagStack::ApplyCost(const UXYSGameplayAbility* Ability,
	const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo->IsNetAuthority())
	{
		if (AController* PC = Ability->GetControllerFromActorInfo())
		{
			if (AXYSPlayerState* PS = Cast<AXYSPlayerState>(PC->PlayerState))
			{
				const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

				const float NumStacksReal = Quantity.GetValueAtLevel(AbilityLevel);
				const int32 NumStacks = FMath::TruncToInt(NumStacksReal);

				PS->RemoveStatTagStack(Tag, NumStacks);
			}
		}
	}
}
