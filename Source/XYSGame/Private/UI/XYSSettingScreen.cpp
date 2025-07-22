// XiaoYao copyright.


#include "UI/XYSSettingScreen.h"

#include "Input/CommonUIInputTypes.h"

#include "Player/XYSLocalPlayer.h"
#include "Settings/XYSGameSettingRegistry.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(XYSSettingScreen)

class UGameSettingRegistry;

void UXYSSettingScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BackHandle = RegisterUIActionBinding(FBindUIActionArgs(BackInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleBackAction)));
	ApplyHandle = RegisterUIActionBinding(FBindUIActionArgs(ApplyInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleApplyAction)));
	CancelChangesHandle = RegisterUIActionBinding(FBindUIActionArgs(CancelChangesInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleCancelChangesAction)));
}

UGameSettingRegistry* UXYSSettingScreen::CreateRegistry()
{
	UXYSGameSettingRegistry* NewRegistry = NewObject<UXYSGameSettingRegistry>();

	if (UXYSLocalPlayer* LocalPlayer = CastChecked<UXYSLocalPlayer>(GetOwningLocalPlayer()))
	{
		NewRegistry->Initialize(LocalPlayer);
	}

	return NewRegistry;
}

void UXYSSettingScreen::HandleBackAction()
{
	if (AttemptToPopNavigation())
	{
		return;
	}

	ApplyChanges();

	DeactivateWidget();
}

void UXYSSettingScreen::HandleApplyAction()
{
	ApplyChanges();
}

void UXYSSettingScreen::HandleCancelChangesAction()
{
	CancelChanges();
}

void UXYSSettingScreen::OnSettingsDirtyStateChanged_Implementation(bool bSettingsDirty)
{
	if (bSettingsDirty)
	{
		if (!GetActionBindings().Contains(ApplyHandle))
		{
			AddActionBinding(ApplyHandle);
		}
		if (!GetActionBindings().Contains(CancelChangesHandle))
		{
			AddActionBinding(CancelChangesHandle);
		}
	}
	else
	{
		RemoveActionBinding(ApplyHandle);
		RemoveActionBinding(CancelChangesHandle);
	}
}
