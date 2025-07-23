// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "XYSPlayerCameraManager.generated.h"


#define XYS_CAMERA_DEFAULT_FOV			(80.0f)
#define XYS_CAMERA_DEFAULT_PITCH_MIN	(-89.0f)
#define XYS_CAMERA_DEFAULT_PITCH_MAX	(89.0f)

class UXYSUICameraManagerComponent;
/**
 * 
 */
UCLASS(notplaceable)
class XYSGAME_API AXYSPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	AXYSPlayerCameraManager(const FObjectInitializer& ObjectInitializer);

	UXYSUICameraManagerComponent* GetUICameraComponent() const;

protected:

	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;

	virtual void DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL, float& YPos) override;

private:
	/** The UI Camera Component, controls the camera when UI is doing something important that gameplay doesn't get priority over. */
	UPROPERTY(Transient)
	TObjectPtr<UXYSUICameraManagerComponent> UICamera;	
};
