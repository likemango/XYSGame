// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "FrontendCommonTabListWidget.generated.h"

/**
 * 
 */
UCLASS()
class FRONTENDUI_API UFrontendCommonTabListWidget : public UCommonTabListWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	
};
