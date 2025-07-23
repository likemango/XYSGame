// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "XYSUICameraManagerComponent.generated.h"


UCLASS(Transient, Within=XYSPlayerCameraManager)
class XYSGAME_API UXYSUICameraManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	static UXYSUICameraManagerComponent* GetComponent(APlayerController* PC);

public:
	UXYSUICameraManagerComponent();	
	virtual void InitializeComponent() override;

	bool IsSettingViewTarget() const { return bUpdatingViewTarget; }
	AActor* GetViewTarget() const { return ViewTarget; }
	void SetViewTarget(AActor* InViewTarget, FViewTargetTransitionParams TransitionParams = FViewTargetTransitionParams());

	bool NeedsToUpdateViewTarget() const;
	void UpdateViewTarget(struct FTViewTarget& OutVT, float DeltaTime);

	void OnShowDebugInfo(AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& YL, float& YPos);

private:
	UPROPERTY(Transient)
	TObjectPtr<AActor> ViewTarget;
	
	UPROPERTY(Transient)
	bool bUpdatingViewTarget;
};
