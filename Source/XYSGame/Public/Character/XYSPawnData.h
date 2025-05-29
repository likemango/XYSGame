// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "XYSPawnData.generated.h"

/**
 * Non-mutable data asset that contains properties used to define a pawn.
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "XYS Pawn Data", ShortTooltip = "Data asset used to define a Pawn."))
class XYSGAME_API UXYSPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
};
