// XiaoYao copyright.


#include "Widgets/Components/FrontendCommonButtonBase.h"

#include "CommonLazyImage.h"
#include "CommonTextBlock.h"
#include "Subsystem/FrontendUISubsystem.h"

void UFrontendCommonButtonBase::SetButtonText(FText InText)
{
	if (CommonTextBlock_ButtonText && !InText.IsEmpty())
	{
		CommonTextBlock_ButtonText->SetText(bUserUpperCaseForButtonText ? InText.ToUpper() : InText);
	}
}

FText UFrontendCommonButtonBase::GetButtonDisplayText() const
{
	if (CommonTextBlock_ButtonText)
	{
		return CommonTextBlock_ButtonText->GetText();
	}
	return FText();
}

void UFrontendCommonButtonBase::SetButtionDisplayImage(const FSlateBrush& InBrush)
{
	if (CommonLazyImage_ButtonImage)
	{
		CommonLazyImage_ButtonImage->SetBrush(InBrush);
	}
}

void UFrontendCommonButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetButtonText(ButtonDisplayText);
}

void UFrontendCommonButtonBase::NativeOnCurrentTextStyleChanged()
{
	if (CommonTextBlock_ButtonText && GetCurrentTextStyleClass())
	{
		CommonTextBlock_ButtonText->SetStyle(GetCurrentTextStyleClass());
	}
	
	Super::NativeOnCurrentTextStyleChanged();
}

void UFrontendCommonButtonBase::NativeOnHovered()
{
	Super::NativeOnHovered();

	UFrontendUISubsystem::Get(this)->OnMenuButtonHovered.Broadcast(this, ButtonDescriptionText);
}

void UFrontendCommonButtonBase::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();

	UFrontendUISubsystem::Get(this)->OnMenuButtonUnHovered.Broadcast(this, FText::GetEmpty());
}
