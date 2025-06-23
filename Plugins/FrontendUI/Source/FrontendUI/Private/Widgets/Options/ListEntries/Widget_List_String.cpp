// XiaoYao copyright.


#include "Widgets/Options/ListEntries/Widget_List_String.h"

#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "Widgets/Components/FrontendCommonRotator.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"

void UWidget_List_String::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CommonButton_PreviousOption->OnClicked().AddUObject(this,&ThisClass::OnPreviousOptionButtonClicked);
	CommonButton_NextOption->OnClicked().AddUObject(this,&ThisClass::OnNextOptionButtonClicked);
}

void UWidget_List_String::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	CachedOwningStringDataObject = CastChecked<UListDataObject_String>(InOwningListDataObject);

	CommonRotator_AvailableOptions->PopulateTextLabels(CachedOwningStringDataObject->GetAvailableOptionsTextArray());
	CommonRotator_AvailableOptions->SetSelectedOptionByText(CachedOwningStringDataObject->GetCurrentDisplayText());
	CommonRotator_AvailableOptions->OnClicked().AddLambda([this](){SelectThisEntryWidget();});
}

void UWidget_List_String::OnPreviousOptionButtonClicked()
{
	if (CachedOwningStringDataObject)
	{
		CachedOwningStringDataObject->BackToPreviousOption();
	}
	SelectThisEntryWidget();
}

void UWidget_List_String::OnNextOptionButtonClicked()
{
	if (CachedOwningStringDataObject)
	{
		CachedOwningStringDataObject->AdvanceToNextOption();
	}
	SelectThisEntryWidget();
}

void UWidget_List_String::OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	if (CachedOwningStringDataObject)
	{
		CommonRotator_AvailableOptions->SetSelectedOptionByText(CachedOwningStringDataObject->GetCurrentDisplayText());
	}
}

