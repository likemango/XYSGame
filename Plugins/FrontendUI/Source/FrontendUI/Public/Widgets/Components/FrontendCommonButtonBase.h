// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "FrontendCommonButtonBase.generated.h"

class UCommonLazyImage;
class UCommonTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class FRONTENDUI_API UFrontendCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetButtonText(FText InText);
	
	UFUNCTION(BlueprintCallable)
	FText GetButtonDisplayText() const;

	UFUNCTION(BlueprintCallable)
	void SetButtionDisplayImage(const FSlateBrush& InBrush);
	
private:
	//~UUserWidget interface
	virtual void NativePreConstruct() override;
	//~

	//~UCommonButtonBase
	virtual void NativeOnCurrentTextStyleChanged() override;
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;
	//~

	/* Bound Widget */
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UCommonTextBlock> CommonTextBlock_ButtonText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional,AllowPrivateAccess = "true"))
	UCommonLazyImage* CommonLazyImage_ButtonImage;
	/* */
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = "true"))
	FText ButtonDisplayText;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = "true"))
	bool bUserUpperCaseForButtonText = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = "true"))
	FText ButtonDescriptionText;
};




