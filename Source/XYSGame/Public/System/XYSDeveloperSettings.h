// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "XYSDeveloperSettings.generated.h"

class UGameplayEffect;
/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="XYS Developer Settings"))
class XYSGAME_API UXYSDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	
#if WITH_EDITORONLY_DATA
	/** A list of common maps that will be accessible via the editor detoolbar */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category=Maps, meta=(AllowedClasses="/Script/Engine.World"))
	TArray<FSoftObjectPath> CommonEditorMaps;
#endif
};
