// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_PushSoftWidgetClassToStack.generated.h"

class UWidget_ActivatableBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAsyncPushWidgetToStack, UWidget_ActivatableBase*, CreatedWidget);

/**
 * 
 */
UCLASS()
class FRONTENDUI_API UAsyncAction_PushSoftWidgetClassToStack : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "FrontendUI|PushSoftWidgetClassToStack", DisplayName="Push Soft Widge tClass To Stack",
		meta=(BlueprintInternalUseOnly="true",WorldContext="WorldContextObject", HidePin="WorldContextObject"))
	static UAsyncAction_PushSoftWidgetClassToStack* PushSoftWidget(const UObject* WorldContextObject, APlayerController* OwningPlayerController,
		UPARAM(meta=(Categories="Frontend.WidgetStack")) FGameplayTag StackTag, TSoftClassPtr<UWidget_ActivatableBase> SoftWidgetClass, bool bFocusOnNewlyPushedWidget);

	virtual void Activate() override;
	
	UPROPERTY(BlueprintAssignable)
	FOnAsyncPushWidgetToStack CreatedBeforePush;

	UPROPERTY(BlueprintAssignable)
	FOnAsyncPushWidgetToStack AfterPush;

private:
	UPROPERTY()
	TWeakObjectPtr<UObject> WorldContextObjectCached;
	UPROPERTY()
	TWeakObjectPtr<APlayerController> OwningPlayerControllerCached;
	UPROPERTY()
	TSoftClassPtr<UWidget_ActivatableBase> SoftWidgetClassCached;
	UPROPERTY()
	FGameplayTag StackTagCached;
	UPROPERTY()
	bool bFocusOnNewlyPushedWidgetCached;;
};
