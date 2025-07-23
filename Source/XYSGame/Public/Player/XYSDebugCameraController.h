// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DebugCameraController.h"
#include "XYSDebugCameraController.generated.h"

/**
 * Used for controlling the debug camera when it is enabled via the cheat manager.
 */
UCLASS()
class XYSGAME_API AXYSDebugCameraController : public ADebugCameraController
{
	GENERATED_BODY()
	
public:

	AXYSDebugCameraController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual void AddCheats(bool bForce) override;
};
