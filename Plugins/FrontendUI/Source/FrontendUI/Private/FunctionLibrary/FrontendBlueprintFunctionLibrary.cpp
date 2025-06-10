// XiaoYao copyright.


#include "FunctionLibrary/FrontendBlueprintFunctionLibrary.h"

#include "GameplayTagContainer.h"
#include "FrontendSettings/FrontendDeveloperSettings.h"

TSoftClassPtr<UWidget_ActivatableBase> UFrontendBlueprintFunctionLibrary::GetFrontendSoftWidgetClassFromDevelopSettingsByTag(FGameplayTag WidgetTag)
{
	checkf(WidgetTag.IsValid(), TEXT("WidgetTag in GetWidgetClassFromDevelopSettingsByTag is null."))

	const UFrontendDeveloperSettings* FrontendDevelopSettings = GetDefault<UFrontendDeveloperSettings>();

	ensureAlwaysMsgf(FrontendDevelopSettings->FrontedWidgetMaps.Contains(WidgetTag), TEXT("FrontendDevelopSettings's FrontedWidgetMaps does not contain tag: %s"), *WidgetTag.ToString());

	return FrontendDevelopSettings->FrontedWidgetMaps.FindRef(WidgetTag);
}
