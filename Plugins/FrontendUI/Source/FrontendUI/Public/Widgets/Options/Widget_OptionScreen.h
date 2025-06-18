// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "Widget_OptionScreen.generated.h"

class UFrontendCommonListView;
class UFrontendCommonTabListWidget;
class UOptionsDataRegistry;
/**
 * 
 */
UCLASS(Abstract, Blueprintable, meta=(DisableNativeTick))
class FRONTENDUI_API UWidget_OptionScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()

protected:
	//~UWidget
	virtual void NativeOnInitialized() override;
	//~

	//~Activatable Widget
	virtual void NativeOnActivated() override;
	//

	/* Bound Widget */
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UFrontendCommonTabListWidget> TabListWidget_OptionsTabs;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UFrontendCommonListView> CommonListView_OptionsList;
	/* Bound Widget */
	
	void HandleResetAction();
	void HandleBackAction();

	UOptionsDataRegistry* GetOrCreateOptionsDataRegistry();

	UFUNCTION()
	void OnTabButtonSelectedCallback(FName TabId);
	
private:
	FUIActionBindingHandle ResetActionBindHandle;
	FUIActionBindingHandle BackActionBindHandle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetDataTableRowHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle BackDataTableRowHandle;

	//Handle the creation of data in the options screen. Direct access to this variable is forbidden
	UPROPERTY(Transient)
	TObjectPtr<UOptionsDataRegistry> CreatedOptionDataRegistry;
};
