// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "XYSWeaponInstance.h"
#include "AbilitySystem/XYSAbilitySourceInterface.h"
#include "XYSRangedWeaponInstance.generated.h"

/**
 * A equipment representing a ranged weapon spawned and applied to a pawn
 */
UCLASS()
class XYSGAME_API UXYSRangedWeaponInstance : public UXYSWeaponInstance, public IXYSAbilitySourceInterface
{
	GENERATED_BODY()

public:
	
	//~ILyraAbilitySourceInterface
	virtual float GetDistanceAttenuation(float Distance, const FGameplayTagContainer* SourceTags = nullptr,const FGameplayTagContainer* TargetTags = nullptr) const override;
	virtual float GetPhysicalMaterialAttenuation(const UPhysicalMaterial* PhysicalMaterial,
		const FGameplayTagContainer* SourceTags = nullptr,const FGameplayTagContainer* TargetTags = nullptr) const override;
	//
};
