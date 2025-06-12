// XiaoYao copyright.


#include "Widgets/Widget_ConfirmScreen.h"

#include "CommonTextBlock.h"
#include "DebugHelper.h"
#include "ICommonInputModule.h"
#include "Components/DynamicEntryBox.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"

UWidget_ConfirmScreen::FConfirmScreenInfoStruct UWidget_ConfirmScreen::CreateOkScreen(const FText& InScreenTitle,const FText& InScreenMessage)
{
	FConfirmScreenInfoStruct ConfirmScreenInfo;
	ConfirmScreenInfo.ScreenTitle = InScreenTitle;
	ConfirmScreenInfo.ScreenMessage = InScreenMessage;
	
	FConfirmScreenButtonInfo OKButtonInfo;
	OKButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Closed;
	OKButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("Ok"));
	ConfirmScreenInfo.AvailableScreenButtons.Add(OKButtonInfo);
	return ConfirmScreenInfo;
}

UWidget_ConfirmScreen::FConfirmScreenInfoStruct UWidget_ConfirmScreen::CreateYesNoScreen(const FText& InScreenTitle,const FText& InScreenMessage)
{
	FConfirmScreenInfoStruct ConfirmScreenInfo;
	ConfirmScreenInfo.ScreenTitle = InScreenTitle;
	ConfirmScreenInfo.ScreenMessage = InScreenMessage;
	
	FConfirmScreenButtonInfo YesButtonInfo;
	YesButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
	YesButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("Yes"));

	FConfirmScreenButtonInfo NoButtonInfo;
	NoButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
	NoButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("No"));
	
	ConfirmScreenInfo.AvailableScreenButtons.Add(YesButtonInfo);
	ConfirmScreenInfo.AvailableScreenButtons.Add(NoButtonInfo);
	
	return ConfirmScreenInfo;
}

UWidget_ConfirmScreen::FConfirmScreenInfoStruct UWidget_ConfirmScreen::CreateOkCancelScreen(const FText& InScreenTitle,const FText& InScreenMessage)
{
	FConfirmScreenInfoStruct ConfirmScreenInfo;
	ConfirmScreenInfo.ScreenTitle = InScreenTitle;
	ConfirmScreenInfo.ScreenMessage = InScreenMessage;
	
	FConfirmScreenButtonInfo OkButtonInfo;
	OkButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
	OkButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("Ok"));

	FConfirmScreenButtonInfo CancelButtonInfo;
	CancelButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
	CancelButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("Cancel"));
	
	ConfirmScreenInfo.AvailableScreenButtons.Add(OkButtonInfo);
	ConfirmScreenInfo.AvailableScreenButtons.Add(CancelButtonInfo);
	
	return ConfirmScreenInfo;
}

void UWidget_ConfirmScreen::InitConfirmScreen(const FConfirmScreenInfoStruct& ScreenInfo, TFunction<void(EConfirmScreenButtonType)> ClickedButtonCallback)
{
	check(CommonTextBlock_Title && CommonTextBlock_Message && DynamicEntryBox_Buttons);

	CommonTextBlock_Title->SetText(ScreenInfo.ScreenTitle);
	CommonTextBlock_Message->SetText(ScreenInfo.ScreenMessage);

	// CommonActivatableWidget can be active/deactivate multi-times, one widget instance can be used multi-times. Clear before use
	if (DynamicEntryBox_Buttons->GetNumEntries() != 0)
	{
		// Add commentMore actions clearing the old buttons the entry box has. The widget type for the entry box is specified in the child widget blueprint.
		DynamicEntryBox_Buttons->Reset<UFrontendCommonButtonBase>([](const UFrontendCommonButtonBase& ExistingButton)
		{
			ExistingButton.OnClicked().Clear();
		});
	}
	check(ScreenInfo.AvailableScreenButtons.Num() != 0);
	
	for (const FConfirmScreenButtonInfo& ButtonInfo : ScreenInfo.AvailableScreenButtons)
	{
		FDataTableRowHandle InputActionRowHandle;
		
		switch (ButtonInfo.ConfirmScreenButtonType) {
		case EConfirmScreenButtonType::Confirmed:
			// InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultClickAction();
			break;
		case EConfirmScreenButtonType::Cancelled:
			InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultBackAction();
			break;
		case EConfirmScreenButtonType::Closed:
			InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultBackAction();
			break;
		case EConfirmScreenButtonType::Unknown:
			break;
		}

		UFrontendCommonButtonBase* AddedButton = DynamicEntryBox_Buttons->CreateEntry<UFrontendCommonButtonBase>();
		AddedButton->SetButtonText(ButtonInfo.ButtonTextToDisplay);
		// AddedButton->SetTriggeredInputAction(InputActionRowHandle);
		AddedButton->SetTriggeringInputAction(InputActionRowHandle);
		AddedButton->OnClicked().AddLambda([ClickedButtonCallback, ButtonInfo, this]()
		{
			ClickedButtonCallback(ButtonInfo.ConfirmScreenButtonType);

			FString EnumStringName = StaticEnum<EConfirmScreenButtonType>()->GetNameStringByValue(static_cast<int64>(ButtonInfo.ConfirmScreenButtonType));
			DebugHelper::Print(FString::Printf(TEXT("ButtonText: %s , ButtonType: %s"), *ButtonInfo.ButtonTextToDisplay.ToString(), *EnumStringName));
			DeactivateWidget();
		});
	}
	if (DynamicEntryBox_Buttons->GetNumEntries() != 0)
	{
		DynamicEntryBox_Buttons->GetAllEntries().Last()->SetFocus();
	}
}

















