// XiaoYao copyright.


#include "Inventory/XYSInventoryItemDefinition.h"

UXYSInventoryItemDefinition::UXYSInventoryItemDefinition(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
}

const UXYSInventoryItemFragment* UXYSInventoryItemDefinition::FindFragmentByClass(TSubclassOf<UXYSInventoryItemFragment> FragmentClass) const
{
	if (FragmentClass)
	{
		for (UXYSInventoryItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}
	return nullptr;
}
