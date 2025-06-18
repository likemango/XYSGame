// XiaoYao copyright.


#include "Widgets/Options/DataAsset_DataListEntryMapping.h"

#include "Widgets/Options/DataObjects/ListDataObject_Base.h"

TSubclassOf<UWidget_ListEntry_Base> UDataAsset_DataListEntryMapping::FindEntryWidgetClassByDataObject(UListDataObject_Base* InDataObject) const
{
	check(InDataObject);

	for (UClass* DataClass = InDataObject->GetClass(); DataClass; DataClass = DataClass->GetSuperClass())
	{
		if (TSubclassOf<UListDataObject_Base> ConvertedDataObjectClass = TSubclassOf<UListDataObject_Base>(DataClass))
		{
			if (DataObjectListEntryMap.Contains(ConvertedDataObjectClass))
			{
				return DataObjectListEntryMap.FindRef(ConvertedDataObjectClass);
			}
		}
	}
	return TSubclassOf<UWidget_ListEntry_Base>();
}
