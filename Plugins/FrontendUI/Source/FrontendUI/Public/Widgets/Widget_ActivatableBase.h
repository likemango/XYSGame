// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Widget_ActivatableBase.generated.h"

class AFrontendPlayerController;
/**
 * 
 */
UCLASS()
class FRONTENDUI_API UWidget_ActivatableBase : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintPure, Category = "Frontend Activatable Widget")
	AFrontendPlayerController* GetOwingFrontendPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TWeakObjectPtr<AFrontendPlayerController> CachedOwningPlayerController;
};
