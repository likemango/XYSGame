// XiaoYao copyright.


#include "Widgets/Options/Widget_OptionScreen.h"

#include "DebugHelper.h"
#include "Input/CommonUIInputTypes.h"

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
