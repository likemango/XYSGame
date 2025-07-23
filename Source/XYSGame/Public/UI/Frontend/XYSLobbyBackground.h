// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "XYSLobbyBackground.generated.h"

/**
 * 
 */
UCLASS(config=EditorPerProjectUserSettings)
class XYSGAME_API UXYSLobbyBackground : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UWorld> BackgroundLevel;
};
