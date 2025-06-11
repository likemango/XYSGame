// XiaoYao copyright.


#include "Weapon/XYSRangedWeaponInstance.h"

float UXYSRangedWeaponInstance::GetDistanceAttenuation(float Distance, const FGameplayTagContainer* SourceTags,const FGameplayTagContainer* TargetTags) const
{
	return 1;
}

float UXYSRangedWeaponInstance::GetPhysicalMaterialAttenuation(const UPhysicalMaterial* PhysicalMaterial,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags) const
{
	return 1;
}
