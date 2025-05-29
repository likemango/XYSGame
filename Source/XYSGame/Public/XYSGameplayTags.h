// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace XYSGameplayTags
{
	XYSGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_Spawned);
	XYSGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataAvailable);
	XYSGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataInitialized);
	XYSGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_GameplayReady);

	XYSGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Behavior_SurvivesDeath);
}
