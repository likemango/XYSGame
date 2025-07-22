// XiaoYao copyright.


#include "UI/XYSSettingScreen.h"

#include "Input/CommonUIInputTypes.h"

void UXYSSettingScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BackHandle = RegisterUIActionBinding(FBindUIActionArgs(BackInputActionData, true,
		FSimpleDelegate::CreateUObject(this, &ThisClass::HandleBackAction)));
	
	ApplyHandle = RegisterUIActionBinding(FBindUIActionArgs(ApplyInputActionData, true,
		FSimpleDelegate::CreateUObject(this, &ThisClass::HandleApplyAction)));
	
	CancelChangesHandle = RegisterUIActionBinding(FBindUIActionArgs(CancelChangesInputActionData, true,
		FSimpleDelegate::CreateUObject(this, &ThisClass::HandleCancelChangesAction)));
}

UGameSettingRegistry* UXYSSettingScreen::CreateRegistry()
{
	return Super::CreateRegistry();
}

void UXYSSettingScreen::HandleBackAction()
{
}

void UXYSSettingScreen::HandleApplyAction()
{
}

void UXYSSettingScreen::HandleCancelChangesAction()
{
}

void UXYSSettingScreen::OnSettingsDirtyStateChanged_Implementation(bool bSettingsDirty)
{
	Super::OnSettingsDirtyStateChanged_Implementation(bSettingsDirty);
}
