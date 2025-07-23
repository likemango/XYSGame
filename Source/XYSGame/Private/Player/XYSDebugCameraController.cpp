// XiaoYao copyright.


#include "Player/XYSDebugCameraController.h"
#include "Player/XYSCheatManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(XYSDebugCameraController)


AXYSDebugCameraController::AXYSDebugCameraController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Use the same cheat class as XYSPlayerController to allow toggling the debug camera through cheats.
	CheatClass = UXYSCheatManager::StaticClass();
}

void AXYSDebugCameraController::AddCheats(bool bForce)
{
	// Mirrors XYSPlayerController's AddCheats() to avoid the player becoming stuck in the debug camera.
#if USING_CHEAT_MANAGER
	Super::AddCheats(true);
#else //#if USING_CHEAT_MANAGER
	Super::AddCheats(bForce);
#endif // #else //#if USING_CHEAT_MANAGER
}

