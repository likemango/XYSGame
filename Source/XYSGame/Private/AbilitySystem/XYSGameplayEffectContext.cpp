// XiaoYao copyright.


#include "AbilitySystem/XYSGameplayEffectContext.h"

#include "AbilitySystem/XYSAbilitySourceInterface.h"
#include "Engine/HitResult.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#if UE_WITH_IRIS
#include "Iris/ReplicationState/PropertyNetSerializerInfoRegistry.h"
#include "Serialization/GameplayEffectContextNetSerializer.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(XYSGameplayEffectContext)

class FArchive;

FXYSGameplayEffectContext* FXYSGameplayEffectContext::ExtractEffectContext(struct FGameplayEffectContextHandle Handle)
{
	FGameplayEffectContext* BaseEffectContext = Handle.Get();
	if ((BaseEffectContext != nullptr) && BaseEffectContext->GetScriptStruct()->IsChildOf(FXYSGameplayEffectContext::StaticStruct()))
	{
		return (FXYSGameplayEffectContext*)BaseEffectContext;
	}

	return nullptr;
}

bool FXYSGameplayEffectContext::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);

	// Not serialized for post-activation use:
	// CartridgeID

	return true;
}

#if UE_WITH_IRIS
namespace UE::Net
{
	// Forward to FGameplayEffectContextNetSerializer
	// Note: If FXYSGameplayEffectContext::NetSerialize() is modified, a custom NetSerializesr must be implemented as the current fallback will no longer be sufficient.
	UE_NET_IMPLEMENT_FORWARDING_NETSERIALIZER_AND_REGISTRY_DELEGATES(XYSGameplayEffectContext, FGameplayEffectContextNetSerializer);
}
#endif

void FXYSGameplayEffectContext::SetAbilitySource(const IXYSAbilitySourceInterface* InObject, float InSourceLevel)
{
	AbilitySourceObject = MakeWeakObjectPtr(Cast<const UObject>(InObject));
	//SourceLevel = InSourceLevel;
}

const IXYSAbilitySourceInterface* FXYSGameplayEffectContext::GetAbilitySource() const
{
	return Cast<IXYSAbilitySourceInterface>(AbilitySourceObject.Get());
}

const UPhysicalMaterial* FXYSGameplayEffectContext::GetPhysicalMaterial() const
{
	if (const FHitResult* HitResultPtr = GetHitResult())
	{
		return HitResultPtr->PhysMaterial.Get();
	}
	return nullptr;
}

