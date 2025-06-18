// XiaoYao copyright.


#include "Widgets/Components/FrontendCommonListView.h"

#include "Editor/WidgetCompilerLog.h"
#include "Widgets/Options/DataAsset_DataListEntryMapping.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"

UUserWidget& UFrontendCommonListView::OnGenerateEntryWidgetInternal(UObject* Item,TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (IsDesignTime())
	{
		return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	}
	TSubclassOf<UWidget_ListEntry_Base> FoundWidgetClass = DataListMapping->FindEntryWidgetClassByDataObject(CastChecked<UListDataObject_Base>(Item));
	
	return GenerateTypedEntry<UWidget_ListEntry_Base>(FoundWidgetClass, OwnerTable);
}

#if WITH_EDITOR
void UFrontendCommonListView::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!DataListMapping)
	{
		CompileLog.Error(FText::FromString(
			TEXT("UDataAsset_DataListEntryMapping has no valid data asset assigned ") + GetClass()->GetName() +
			TEXT(" needs a valid data asset to function property")));
	}
}
#endif
