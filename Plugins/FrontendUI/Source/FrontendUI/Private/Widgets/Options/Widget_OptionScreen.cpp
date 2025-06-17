// XiaoYao copyright.


#include "Widgets/Options/Widget_OptionScreen.h"

#include "DebugHelper.h"
#include "Input/CommonUIInputTypes.h"
#include "Widgets/Options/FrontendCommonTabListWidget.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Options/DataObjects/OptionsDataRegistry.h"

void UWidget_OptionScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(!ResetDataTableRowHandle.IsNull() && !BackDataTableRowHandle.IsNull())

	FBindUIActionArgs BindResetActionArgs(ResetDataTableRowHandle, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleResetAction));
	ResetActionBindHandle = RegisterUIActionBinding(BindResetActionArgs);

	FBindUIActionArgs BindBackActionArgs(BackDataTableRowHandle, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleBackAction));
	BackActionBindHandle = RegisterUIActionBinding(BindBackActionArgs);
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
		if (TabListWidget_OptionTabs->GetTabButtonBaseByID(TabID))
			continue;

		TabListWidget_OptionTabs->RequestRegisterTab(TabID, TabCollection->GetDataDisplayName());
	}
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
