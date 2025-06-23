// XiaoYao copyright.


#include "Widgets/Options/Widget_OptionDetailsView.h"

#include "CommonLazyImage.h"
#include "CommonRichTextBlock.h"
#include "CommonTextBlock.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"

void UWidget_OptionDetailsView::UpdateDetailsViewInfo(UListDataObject_Base* InDataObject, const FString& InEntryWidgetClassName)
{
	if (!InDataObject) return;

	CommonTextBlock_Title->SetText(InDataObject->GetDataDisplayName());
	if (!InDataObject->GetSoftDescriptionImage().IsNull())
	{
		CommonLazyImage_DescriptionImage->SetBrushFromLazyTexture(InDataObject->GetSoftDescriptionImage());
		CommonLazyImage_DescriptionImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	CommonRichText_Description->SetText(InDataObject->GetDescriptionRichText());

	const FString DynamicDetails = FString::Printf(
		TEXT("Data Object Class: <Bold>%s</>\n\nEntry Widget Class:<Bold>%s</>"),
		*InDataObject->GetClass()->GetName(),
		*InEntryWidgetClassName
	);

	CommonRichText_DynamicDetails->SetText(FText::FromString(DynamicDetails));

	CommonRichText_DisabledReason->SetText(InDataObject->GetDisabledRichText());
}

void UWidget_OptionDetailsView::ClearDetailsViewInfo()
{
	CommonTextBlock_Title->SetText(FText::GetEmpty());
	CommonLazyImage_DescriptionImage->SetVisibility(ESlateVisibility::Collapsed);
	CommonRichText_Description->SetText(FText::GetEmpty());
	CommonRichText_DynamicDetails->SetText(FText::GetEmpty());
	CommonRichText_DisabledReason->SetText(FText::GetEmpty());
}

void UWidget_OptionDetailsView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ClearDetailsViewInfo();
}
