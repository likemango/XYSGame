// XiaoYao copyright.


#include "FunctionLibrary/FrontendBlueprintFunctionLibrary.h"

#include "GameplayTagContainer.h"
#include "FrontendSettings/FrontendDeveloperSettings.h"

TSoftClassPtr<UWidget_ActivatableBase> UFrontendBlueprintFunctionLibrary::GetFrontendSoftWidgetClassByTag(FGameplayTag WidgetTag)
{
	checkf(WidgetTag.IsValid(), TEXT("WidgetTag in GetWidgetClassFromDevelopSettingsByTag is null."))

	const UFrontendDeveloperSettings* FrontendDevelopSettings = GetDefault<UFrontendDeveloperSettings>();

	ensureAlwaysMsgf(FrontendDevelopSettings->FrontedWidgetMaps.Contains(WidgetTag), TEXT("FrontendDevelopSettings's FrontedWidgetMaps does not contain tag: %s"), *WidgetTag.ToString());

	return FrontendDevelopSettings->FrontedWidgetMaps.FindRef(WidgetTag);
}

TSoftClassPtr<UWidget_PrimaryLayout> UFrontendBlueprintFunctionLibrary::GetPrimaryLayoutWidgetClass()
{
	const UFrontendDeveloperSettings* FrontendDeveloperSettings = GetDefault<UFrontendDeveloperSettings>();
	return FrontendDeveloperSettings->PrimaryLayoutClass;
}


TSoftObjectPtr<UTexture2D> UFrontendBlueprintFunctionLibrary::GetOptionsSoftImageByTag(FGameplayTag InImageTag)
{
	const UFrontendDeveloperSettings* FrontendDeveloperSettings = GetDefault<UFrontendDeveloperSettings>();

	checkf(FrontendDeveloperSettings->OptionsScreenSoftImageMap.Contains(InImageTag),TEXT("Could not find an image accociated with tag %s"),*InImageTag.ToString());

	return FrontendDeveloperSettings->OptionsScreenSoftImageMap.FindRef(InImageTag);
}
