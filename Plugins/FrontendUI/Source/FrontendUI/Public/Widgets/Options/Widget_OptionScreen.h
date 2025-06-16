// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "Widget_OptionScreen.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, meta=(DisableNativeTick))
class FRONTENDUI_API UWidget_OptionScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	void HandleResetAction();
	void HandleBackAction();
	
private:
	FUIActionBindingHandle ResetActionBindHandle;
	FUIActionBindingHandle BackActionBindHandle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetDataTableRowHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle BackDataTableRowHandle;
};
