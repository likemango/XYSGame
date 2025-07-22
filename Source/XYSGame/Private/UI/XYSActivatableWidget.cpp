// XiaoYao copyright.

#include "UI/XYSActivatableWidget.h"

#include "Editor/WidgetCompilerLog.h"

UXYSActivatableWidget::UXYSActivatableWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

TOptional<FUIInputConfig> UXYSActivatableWidget::GetDesiredInputConfig() const
{
	switch (InputConfig)
	{
	case EXYSWidgetInputMode::GameAndMenu:
		return FUIInputConfig(ECommonInputMode::All, GameMouseCaptureMode);
	case EXYSWidgetInputMode::Game:
		return FUIInputConfig(ECommonInputMode::Game, GameMouseCaptureMode);
	case EXYSWidgetInputMode::Menu:
		return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
	case EXYSWidgetInputMode::Default:
		default:
		return TOptional<FUIInputConfig>();
	}
}

#if WITH_EDITOR
void UXYSActivatableWidget::ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree,
	class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledWidgetTree(BlueprintWidgetTree, CompileLog);

	if (!GetClass()->IsFunctionImplementedInScript(GET_FUNCTION_NAME_CHECKED(UXYSActivatableWidget, BP_GetDesiredFocusTarget)))
	{
		if (GetParentNativeClass(GetClass()) == UXYSActivatableWidget::StaticClass())
		{
			CompileLog.Warning(FText::FromString(TEXT("GetDesiredFocusTarget wasn't implemented, you're going to have trouble using gamepads on this screen.")));
		}
		else
		{
			//TODO - Note for now, because we can't guarantee it isn't implemented in a native subclass of this one.
			CompileLog.Note(FText::FromString(TEXT("GetDesiredFocusTarget wasn't implemented, you're going to have trouble using gamepads on this screen.  If it was implemented in the native base class you can ignore this message.")));
		}
	}
}
#endif
