// XiaoYao copyright.


#include "AbilitySystem/XYSGameplayAbilityTargetData_SingleTargetHit.h"
#include "AbilitySystem/XYSGameplayEffectContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(XYSGameplayAbilityTargetData_SingleTargetHit)

struct FGameplayEffectContextHandle;

//////////////////////////////////////////////////////////////////////

void FXYSGameplayAbilityTargetData_SingleTargetHit::AddTargetDataToContext(FGameplayEffectContextHandle& Context, bool bIncludeActorArray) const
{
	FGameplayAbilityTargetData_SingleTargetHit::AddTargetDataToContext(Context, bIncludeActorArray);

	// Add game-specific data
	if (FXYSGameplayEffectContext* TypedContext = FXYSGameplayEffectContext::ExtractEffectContext(Context))
	{
		TypedContext->CartridgeID = CartridgeID;
	}
}

bool FXYSGameplayAbilityTargetData_SingleTargetHit::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	FGameplayAbilityTargetData_SingleTargetHit::NetSerialize(Ar, Map, bOutSuccess);

	Ar << CartridgeID;

	return true;
}

