// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "FrontendTypes/FrontendTypes.h"
#include "Widget_ActivatableBase.h"
#include "Widget_ConfirmScreen.generated.h"

class UDynamicEntryBox;
class UCommonTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class FRONTENDUI_API UWidget_ConfirmScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()

public:
	struct FConfirmScreenButtonInfo
	{
		EConfirmScreenButtonType ConfirmScreenButtonType = EConfirmScreenButtonType::Unknown;
		FText ButtonTextToDisplay = FText::GetEmpty();
	};
	struct FConfirmScreenInfoStruct
	{
		FText ScreenTitle = FText::GetEmpty();
		FText ScreenMessage = FText::GetEmpty();
		TArray<FConfirmScreenButtonInfo> AvailableScreenButtons {};
	};

	static FConfirmScreenInfoStruct CreateOkScreen(const FText& InScreenTitle, const FText& InScreenMessage);
	static FConfirmScreenInfoStruct CreateYesNoScreen(const FText& InScreenTitle, const FText& InScreenMessage);
	static FConfirmScreenInfoStruct CreateOkCancelScreen(const FText& InScreenTitle, const FText& InScreenMessage);

	// get call when this widget is constructed before it's pushed to the modal stack
	void InitConfirmScreen(const FConfirmScreenInfoStruct& ScreenInfo, TFunction<void(EConfirmScreenButtonType)> ClickedButtonCallback);

protected:
	//~Begin UCommonActivatableWidget interface
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	//~End
	
private:
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* CommonTextBlock_Title;

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* CommonTextBlock_Message;

	UPROPERTY(meta = (BindWidget))
	UDynamicEntryBox* DynamicEntryBox_Buttons;
};
