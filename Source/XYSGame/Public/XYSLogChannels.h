// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"

XYSGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogXYSGame, Log, All);
XYSGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogXYSAbilitySystem, Log, All);
XYSGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogXYSInventory, Log, All);
XYSGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogXYSTeams, Log, All);

XYSGAME_API FString GetClientServerContextString(UObject* ContextObject = nullptr);
