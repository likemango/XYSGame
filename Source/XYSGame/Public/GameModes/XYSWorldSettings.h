// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "XYSWorldSettings.generated.h"

class UXYSExperienceDefinition;
/**
 * 
 */
UCLASS()
class XYSGAME_API AXYSWorldSettings : public AWorldSettings
{
	GENERATED_BODY()

public:
	AXYSWorldSettings(const FObjectInitializer& ObjectInitializer);

#if WITH_EDITOR
	virtual void CheckForErrors() override;
#endif

public:
	// Returns the default experience to use when a server opens this map if it is not overridden by the user-facing experience
	FPrimaryAssetId GetDefaultGameplayExperience() const;

protected:
	// The default experience to use when a server opens this map if it is not overridden by the user-facing experience
	UPROPERTY(EditDefaultsOnly, Category=GameMode)
	TSoftClassPtr<UXYSExperienceDefinition> DefaultGameplayExperience;

public:
#if WITH_EDITORONLY_DATA
	// Is this level part of a front-end or other standalone experience?
	// When set, the net mode will be forced to Standalone when you hit Play in the editor
	UPROPERTY(EditDefaultsOnly, Category=PIE)
	bool ForceStandaloneNetMode = false;
#endif
};
