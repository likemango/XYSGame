// XiaoYao copyright.


#include "Widgets/Components/FrontendCommonTabListWidget.h"

#include "Editor/WidgetCompilerLog.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"

void UFrontendCommonTabListWidget::RequestRegisterTab(const FName& InTabID, const FText& InTabDisplayName)
{
	RegisterTab(InTabID, TabButtonEntryWidgetClass, nullptr);

	if (UFrontendCommonButtonBase* CreatedButton = Cast<UFrontendCommonButtonBase>(GetTabButtonBaseByID(InTabID)))
	{
		CreatedButton->SetButtonText(InTabDisplayName);
	}
}

void UFrontendCommonTabListWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

#if WITH_EDITOR
void UFrontendCommonTabListWidget::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!TabButtonEntryWidgetClass)
	{
		CompileLog.Error(FText::FromString(
			TEXT("The variable TabButtonEntryWidgetClass has no valid entry specified. ") +
			GetClass()->GetName() +
			TEXT(" needs a valid entry widget class to function properly")
		));
	}
}
#endif
