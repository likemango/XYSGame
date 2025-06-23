// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "FrontendTypes.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Widget_ListEntry_Base.generated.h"

class UListDataObject_Base;
class UCommonTextBlock;
/**
 * 
 */
UCLASS(Abstract)
class FRONTENDUI_API UWidget_ListEntry_Base : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On List Entry Widget Hovered"))
	void BP_OnListEntryWidgetHovered(bool bWasHovered,bool bIsEntryWidgetStillSelected);
	void NativeOnListEntryWidgetHovered(bool bWasHovered);

protected:
	//~Begin IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	//~End

	// The child class should override this function to handle the initialization, supper should be called
	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningDataObject);
	//The child class should override this function to update the UI values after the data object has been modified. Super call is not neededAdd commentMore actions
	virtual void OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason);

	void SelectThisEntryWidget();
	
private:
	//***** Bound Widgets ***** //Add commentMore actions
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional,AllowPrivateAccess = "true"))
	UCommonTextBlock* CommonText_SettingDisplayName;
	//***** Bound Widgets ***** //
};
