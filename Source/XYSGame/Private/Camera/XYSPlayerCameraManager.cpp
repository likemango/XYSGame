// XiaoYao copyright.


#include "Camera/XYSPlayerCameraManager.h"
#include "Async/TaskGraphInterfaces.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Camera/XYSCameraComponent.h"
#include "Camera/XYSUICameraManagerComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(XYSPlayerCameraManager)

class FDebugDisplayInfo;

static FName UICameraComponentName(TEXT("UICamera"));

AXYSPlayerCameraManager::AXYSPlayerCameraManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultFOV = XYS_CAMERA_DEFAULT_FOV;
	ViewPitchMin = XYS_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = XYS_CAMERA_DEFAULT_PITCH_MAX;

	UICamera = CreateDefaultSubobject<UXYSUICameraManagerComponent>(UICameraComponentName);
}

UXYSUICameraManagerComponent* AXYSPlayerCameraManager::GetUICameraComponent() const
{
	return UICamera;
}

void AXYSPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	// If the UI Camera is looking at something, let it have priority.
	if (UICamera->NeedsToUpdateViewTarget())
	{
		Super::UpdateViewTarget(OutVT, DeltaTime);
		UICamera->UpdateViewTarget(OutVT, DeltaTime);
		return;
	}

	Super::UpdateViewTarget(OutVT, DeltaTime);
}

void AXYSPlayerCameraManager::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL, float& YPos)
{
	check(Canvas);

	FDisplayDebugManager& DisplayDebugManager = Canvas->DisplayDebugManager;

	DisplayDebugManager.SetFont(GEngine->GetSmallFont());
	DisplayDebugManager.SetDrawColor(FColor::Yellow);
	DisplayDebugManager.DrawString(FString::Printf(TEXT("XYSPlayerCameraManager: %s"), *GetNameSafe(this)));

	Super::DisplayDebug(Canvas, DebugDisplay, YL, YPos);

	const APawn* Pawn = (PCOwner ? PCOwner->GetPawn() : nullptr);

	if (const UXYSCameraComponent* CameraComponent = UXYSCameraComponent::FindCameraComponent(Pawn))
	{
		CameraComponent->DrawDebug(Canvas);
	}
}

