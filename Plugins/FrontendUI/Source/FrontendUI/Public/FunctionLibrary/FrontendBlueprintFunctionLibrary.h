// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FrontendBlueprintFunctionLibrary.generated.h"

class UWidget_ActivatableBase;
struct FGameplayTag;
/**
 * 
 */
UCLASS()
class FRONTENDUI_API UFrontendBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="Frontend Blueprint Function Library")
	static TSoftClassPtr<UWidget_ActivatableBase> GetFrontendSoftWidgetClassFromDevelopSettingsByTag(UPARAM(meta=(Categories="Frontend.Widget")) FGameplayTag WidgetTag);

	UFUNCTION(BlueprintPure, Category = "Frontend Function Library")
	static TSoftObjectPtr<UTexture2D> GetOptionsSoftImageByTag(UPARAM(meta = (Categories = "Frontend.Image")) FGameplayTag InImageTag);
};
