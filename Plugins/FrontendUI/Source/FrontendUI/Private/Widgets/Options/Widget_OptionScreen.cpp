// XiaoYao copyright.


#include "Widgets/Options/Widget_OptionScreen.h"

#include "DebugHelper.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Input/CommonUIInputTypes.h"
#include "Widgets/Components/FrontendCommonListView.h"
#include "Widgets/Options/FrontendCommonTabListWidget.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Options/DataObjects/OptionsDataRegistry.h"
#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"

void UWidget_OptionScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(!ResetDataTableRowHandle.IsNull() && !BackDataTableRowHandle.IsNull())

	FBindUIActionArgs BindResetActionArgs(ResetDataTableRowHandle, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleResetAction));
	ResetActionBindHandle = RegisterUIActionBinding(BindResetActionArgs);

	FBindUIActionArgs BindBackActionArgs(BackDataTableRowHandle, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleBackAction));
	BackActionBindHandle = RegisterUIActionBinding(BindBackActionArgs);

	TabListWidget_OptionsTabs->OnTabSelected.AddDynamic(this, &ThisClass::OnTabButtonSelected);
	CommonListView_OptionsList->OnItemIsHoveredChanged().AddUObject(this,&ThisClass::OnListViewItemHovered);
	CommonListView_OptionsList->OnItemSelectionChanged().AddUObject(this,&ThisClass::OnListViewItemSelected);
}

void UWidget_OptionScreen::HandleResetAction()
{
	DebugHelper::Print(TEXT("Widget_OptionScreen::HandleResetAction"));
}

void UWidget_OptionScreen::HandleBackAction()
{
	DeactivateWidget();
}

void UWidget_OptionScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	for (UListDataObject_Collection* TabCollection : GetOrCreateOptionsDataRegistry()->GetRegisteredOptionsTabCollections())
	{
		if (!TabCollection)
			continue;
		
		const FName TabID = TabCollection->GetDataID();
		if (TabListWidget_OptionsTabs->GetTabButtonBaseByID(TabID))
			continue;

		TabListWidget_OptionsTabs->RequestRegisterTab(TabID, TabCollection->GetDataDisplayName());
	}
}

void UWidget_OptionScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	
	UFrontendGameUserSettings::Get()->ApplySettings(true);
}

UOptionsDataRegistry* UWidget_OptionScreen::GetOrCreateOptionsDataRegistry()
{
	if (!CreatedOptionDataRegistry)
	{
		CreatedOptionDataRegistry = NewObject<UOptionsDataRegistry>();
		CreatedOptionDataRegistry->InitOptionsDataRegistry(GetOwningLocalPlayer());
	}
	checkf(CreatedOptionDataRegistry,TEXT("Data registry for options screen is not valid"));
	return CreatedOptionDataRegistry;
}

void UWidget_OptionScreen::OnTabButtonSelected(FName TabId)
{
	TArray<UListDataObject_Base*> FoundListSourceItems = GetOrCreateOptionsDataRegistry()->GetListSourceItemsBySelectedTabID(TabId);

	CommonListView_OptionsList->SetListItems(FoundListSourceItems);
	CommonListView_OptionsList->RequestRefresh();

	if (CommonListView_OptionsList->GetNumItems() != 0)
	{
		CommonListView_OptionsList->NavigateToIndex(0);
		CommonListView_OptionsList->SetSelectedIndex(0);
	}
}

void UWidget_OptionScreen::OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered)
{
	UWidget_ListEntry_Base* HoveredEntryWidget = CommonListView_OptionsList->GetEntryWidgetFromItem<UWidget_ListEntry_Base>(InHoveredItem);

	check(HoveredEntryWidget);

	HoveredEntryWidget->NativeOnListEntryWidgetHovered(bWasHovered);
}

void UWidget_OptionScreen::OnListViewItemSelected(UObject* InSelectedItem)
{
}
