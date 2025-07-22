// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "FrontendTypes/FrontendTypes.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "Widget_OptionScreen.generated.h"

class UListDataObject_Base;
class UWidget_OptionDetailsView;
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
	virtual void NativeOnDeactivated() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	//

	/* Bound Widget */
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UFrontendCommonTabListWidget> TabListWidget_OptionsTabs;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UFrontendCommonListView> CommonListView_OptionsList;
	UPROPERTY(meta = (BindWidget))
	UWidget_OptionDetailsView* DetailsView_ListEntryInfo;
	/* Bound Widget */
	
	void OnResetBoundActionTriggered();
	void OnBackBoundActionTriggered();

	UOptionsDataRegistry* GetOrCreateOptionsDataRegistry();

	UFUNCTION()
	void OnTabButtonSelected(FName TabId);

	void OnListViewItemHovered(UObject* InHoveredItem,bool bWasHovered);
	void OnListViewItemSelected(UObject* InSelectedItem);

	FString TryGetEntryWidgetClassName(UObject* InOwningListItem) const;

	void OnListViewListDataModified(UListDataObject_Base* ModifiedData, EOptionsListDataModifyReason ModifyReason);
	
private:
	FUIActionBindingHandle ResetActionHandle;
	FUIActionBindingHandle BackActionHandle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetDataTableRowHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle BackDataTableRowHandle;

	//Handle the creation of data in the options screen. Direct access to this variable is forbidden
	UPROPERTY(Transient)
	TObjectPtr<UOptionsDataRegistry> CreatedOptionDataRegistry;
	
	UPROPERTY(Transient)
	TArray<UListDataObject_Base*> ResettableDataArray;

	bool bIsResettingData = false;
};
