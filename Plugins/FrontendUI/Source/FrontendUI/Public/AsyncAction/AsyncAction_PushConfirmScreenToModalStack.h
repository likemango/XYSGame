// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "FrontendTypes/FrontendTypes.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_PushConfirmScreenToModalStack.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfirmButtonClickedDelegate, EConfirmScreenButtonType, ConfirmScreenButtonType);

/**
 * 
 */
UCLASS()
class FRONTENDUI_API UAsyncAction_PushConfirmScreenToModalStack : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "FrontendUI", DisplayName="Push ConfirmScreen To Modal Stack",
		meta=(BlueprintInternalUseOnly="true",WorldContext="WorldContextObject", HidePin="WorldContextObject"))
	static UAsyncAction_PushConfirmScreenToModalStack* PushConfirmScreen(const UObject* WorldContextObject, EConfirmScreenType InScreenType,
		const FText& InScreenTitle, const FText& InScreenMessage);

	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FOnConfirmButtonClickedDelegate OnConfirmButtonClicked;
	
private:
	UPROPERTY()
	TWeakObjectPtr<UObject> WorldContextObjectCached;
	UPROPERTY()
	EConfirmScreenType ScreenTypeCached;
	UPROPERTY()
	FText ScreenTileCached;
	UPROPERTY()
	FText ScreenMessageCached;
};
