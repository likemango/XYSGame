// XiaoYao copyright.


#include "Widgets/Options/Widget_OptionScreen.h"

#include "Debug.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Input/CommonUIInputTypes.h"
#include "Subsystem/FrontendUISubsystem.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "Widgets/Components/FrontendCommonListView.h"
#include "Widgets/Components/FrontendCommonTabListWidget.h"
#include "Widgets/Options/Widget_OptionDetailsView.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Options/DataObjects/OptionsDataRegistry.h"
#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"

void UWidget_OptionScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(!ResetDataTableRowHandle.IsNull() && !BackDataTableRowHandle.IsNull())

	FBindUIActionArgs BindResetActionArgs(ResetDataTableRowHandle, FSimpleDelegate::CreateUObject(this, &ThisClass::OnResetBoundActionTriggered));
	ResetActionHandle = RegisterUIActionBinding(BindResetActionArgs);

	FBindUIActionArgs BindBackActionArgs(BackDataTableRowHandle, FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionTriggered));
	BackActionHandle = RegisterUIActionBinding(BindBackActionArgs);

	TabListWidget_OptionsTabs->OnTabSelected.AddUniqueDynamic(this, &ThisClass::OnTabButtonSelected);
	CommonListView_OptionsList->OnItemIsHoveredChanged().AddUObject(this,&ThisClass::OnListViewItemHovered);
	CommonListView_OptionsList->OnItemSelectionChanged().AddUObject(this,&ThisClass::OnListViewItemSelected);
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

void UWidget_OptionScreen::OnResetBoundActionTriggered()
{
	if (ResettableDataArray.IsEmpty()) return;

	UCommonButtonBase* SelectedTabButton = TabListWidget_OptionsTabs->GetTabButtonBaseByID(TabListWidget_OptionsTabs->GetActiveTab());
	const FString SelectedTabButtonName = CastChecked<UFrontendCommonButtonBase>(SelectedTabButton)->GetButtonDisplayText().ToString();

	UFrontendUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
		EConfirmScreenType::YesNo,
		FText::FromString(TEXT("Reset")),
		FText::FromString(TEXT("Are you sure you want to reset all the settings under the ") + SelectedTabButtonName + TEXT(" tab?")),
		[this](EConfirmScreenButtonType ClickedButtonType)
		{
			if (ClickedButtonType != EConfirmScreenButtonType::Confirmed)
				return;
			if (ClickedButtonType != EConfirmScreenButtonType::Confirmed)
			{
				return;
			}

			bIsResettingData = true;
			bool bHasDataFailedToReset = false;

			for (UListDataObject_Base* DataToReset : ResettableDataArray)
			{
				if (!DataToReset)
				{
					continue;
				}

				if (DataToReset->TryResetBackToDefaultValue())
				{
					Debug::Print(DataToReset->GetDataDisplayName().ToString() + TEXT(" was reset"));
				}
				else
				{
					bHasDataFailedToReset = true;
					Debug::Print(DataToReset->GetDataDisplayName().ToString() + TEXT(" failed to reset"));
				}
			}

			if (!bHasDataFailedToReset)
			{
				ResettableDataArray.Empty();
				RemoveActionBinding(ResetActionHandle);
			}

			bIsResettingData = false;
		}
	);
}

void UWidget_OptionScreen::OnBackBoundActionTriggered()
{
	DeactivateWidget();
}

UWidget* UWidget_OptionScreen::NativeGetDesiredFocusTarget() const
{
	if (UObject* SelectedObject = CommonListView_OptionsList->GetSelectedItem())
	{
		if (UUserWidget* SelectedEntryWidget = CommonListView_OptionsList->GetEntryWidgetFromItem(SelectedObject))
		{
			return SelectedEntryWidget;
		}
	}
	return Super::NativeGetDesiredFocusTarget();
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
	DetailsView_ListEntryInfo->ClearDetailsViewInfo();
	
	TArray<UListDataObject_Base*> FoundListSourceItems = GetOrCreateOptionsDataRegistry()->GetListSourceItemsBySelectedTabID(TabId);

	CommonListView_OptionsList->SetListItems(FoundListSourceItems);
	CommonListView_OptionsList->RequestRefresh();

	if (CommonListView_OptionsList->GetNumItems() != 0)
	{
		CommonListView_OptionsList->SetSelectedIndex(0);
		CommonListView_OptionsList->NavigateToIndex(0);
	}

	ResettableDataArray.Empty();

	for (UListDataObject_Base* FoundListSourceItem : FoundListSourceItems)
	{
		if (!FoundListSourceItem)
		{
			continue;
		}

		if (!FoundListSourceItem->OnListDataModified.IsBoundToObject(this))
		{
			FoundListSourceItem->OnListDataModified.AddUObject(this,&ThisClass::OnListViewListDataModified);
		}

		if (FoundListSourceItem->CanResetBackToDefaultValue())
		{
			ResettableDataArray.AddUnique(FoundListSourceItem);
		}
	}

	if (ResettableDataArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionHandle);
	}
	else
	{
		if (!GetActionBindings().Contains(ResetActionHandle))
		{
			AddActionBinding(ResetActionHandle);
		}
	}
}

void UWidget_OptionScreen::OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered)
{
	if (!InHoveredItem)
	{
		return;
	}

	UWidget_ListEntry_Base* HoveredEntryWidget = CommonListView_OptionsList->GetEntryWidgetFromItem<UWidget_ListEntry_Base>(InHoveredItem);

	check(HoveredEntryWidget);

	HoveredEntryWidget->NativeOnListEntryWidgetHovered(bWasHovered);

	if (bWasHovered)
	{
		DetailsView_ListEntryInfo->UpdateDetailsViewInfo(CastChecked<UListDataObject_Base>(InHoveredItem),TryGetEntryWidgetClassName(InHoveredItem));
	}
	else
	{
		if (UListDataObject_Base* SelectedItem = CommonListView_OptionsList->GetSelectedItem<UListDataObject_Base>())
		{
			DetailsView_ListEntryInfo->UpdateDetailsViewInfo(SelectedItem,TryGetEntryWidgetClassName(SelectedItem));
		}
	}
}

void UWidget_OptionScreen::OnListViewItemSelected(UObject* InSelectedItem)
{
	if (!InSelectedItem)
		return;

	DetailsView_ListEntryInfo->UpdateDetailsViewInfo(CastChecked<UListDataObject_Base>(InSelectedItem),TryGetEntryWidgetClassName(InSelectedItem));
}

FString UWidget_OptionScreen::TryGetEntryWidgetClassName(UObject* InOwningListItem) const
{
	if (UUserWidget* FoundEntryWidget = CommonListView_OptionsList->GetEntryWidgetFromItem(InOwningListItem))
	{
		return FoundEntryWidget->GetClass()->GetName();
	}
	return TEXT("Entry Widget Not Valid");
}

void UWidget_OptionScreen::OnListViewListDataModified(UListDataObject_Base* ModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	if (!ModifiedData || bIsResettingData) return;

	if (ModifiedData->CanResetBackToDefaultValue())
	{
		ResettableDataArray.AddUnique(ModifiedData);
		if (!GetActionBindings().Contains(ResetActionHandle))
		{
			AddActionBinding(ResetActionHandle);
		}
	}
	else
	{
		if (ResettableDataArray.Contains(ModifiedData))
		{
			ResettableDataArray.Remove(ModifiedData);
		}
	}
	if (ResettableDataArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionHandle);
	}
}
