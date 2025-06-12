// XiaoYao copyright.


#include "Equipment/XYSGameplayAbility_FromEquipment.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include "Equipment/XYSEquipmentInstance.h"
#include "Inventory/XYSInventoryItemInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(XYSGameplayAbility_FromEquipment)

UXYSGameplayAbility_FromEquipment::UXYSGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UXYSEquipmentInstance* UXYSGameplayAbility_FromEquipment::GetAssociatedEquipment() const
{
	if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
	{
		return Cast<UXYSEquipmentInstance>(Spec->SourceObject.Get());
	}

	return nullptr;
}

UXYSInventoryItemInstance* UXYSGameplayAbility_FromEquipment::GetAssociatedItem() const
{
	if (UXYSEquipmentInstance* Equipment = GetAssociatedEquipment())
	{
		return Cast<UXYSInventoryItemInstance>(Equipment->GetInstigator());
	}
	return nullptr;
}


#if WITH_EDITOR
EDataValidationResult UXYSGameplayAbility_FromEquipment::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	PRAGMA_DISABLE_DEPRECATION_WARNINGS
		if (InstancingPolicy == EGameplayAbilityInstancingPolicy::NonInstanced)
			PRAGMA_ENABLE_DEPRECATION_WARNINGS
				{
					Context.AddError(NSLOCTEXT("XYS", "EquipmentAbilityMustBeInstanced", "Equipment ability must be instanced"));
					Result = EDataValidationResult::Invalid;
				}

	return Result;
}

#endif
