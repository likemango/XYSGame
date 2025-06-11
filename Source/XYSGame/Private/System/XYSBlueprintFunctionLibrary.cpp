// XiaoYao copyright.


#include "System/XYSBlueprintFunctionLibrary.h"

#include "Inventory/XYSInventoryItemDefinition.h"

///////////////////////////////////////////
///InventorySystem
const UXYSInventoryItemFragment* UXYSInventoryFunctionLibrary::FindFragmentFromItemDefinition(
	TSubclassOf<UXYSInventoryItemDefinition> ItemDef, TSubclassOf<UXYSInventoryItemFragment> FragmentClass)
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		return GetDefault<UXYSInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}
	return nullptr;
}
///////////////////////////////////////////

