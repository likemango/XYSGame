// XiaoYao copyright.


#include "AbilitySystem/XYSAbilitySystemGlobals.h"

#include "AbilitySystem/XYSGameplayEffectContext.h"

UXYSAbilitySystemGlobals::UXYSAbilitySystemGlobals(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
}

FGameplayEffectContext* UXYSAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FXYSGameplayEffectContext();
}
