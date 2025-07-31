// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "FrontendDeveloperSettings.generated.h"

class UWidget_PrimaryLayout;
class UWidget;
class UWidget_ActivatableBase;
/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, DisplayName="Frontend Developer Settings")
class FRONTENDUI_API UFrontendDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category="Widget Reference", meta=(ForceInlineRow, Categories = "Frontend.Widget"))
	TMap<FGameplayTag, TSoftClassPtr<UWidget_ActivatableBase>> FrontedWidgetMaps;

	UPROPERTY(Config, EditAnywhere, Category = "Options Image Reference", meta = (ForceInlineRow, Categories = "Frontend.Image"))
	TMap< FGameplayTag, TSoftObjectPtr<UTexture2D> > OptionsScreenSoftImageMap;

	UPROPERTY(Config, EditAnywhere, Category="Widget Reference")
	TSoftClassPtr<UWidget_PrimaryLayout> PrimaryLayoutClass;
};
