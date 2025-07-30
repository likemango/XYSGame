// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "XYSActivatableWidget.h"
#include "XYSHUDLayout.generated.h"

class UXYSControllerDisconnectedScreen;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable, meta=(DisplayName="XYS HUD Layout"), Category="XYS|HUD")
class XYSGAME_API UXYSHUDLayout : public UXYSActivatableWidget
{
	GENERATED_BODY()

public:
	UXYSHUDLayout(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

protected:
	void HandleEscapeAction() const;
	
	/**
	 * The menu to be displayed when the user presses the "Pause" or "Escape" button 
	 */
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCommonActivatableWidget> EscapeMenuClass;

	/** Handle from the FSTicker for when we want to process the controller state of our player */
	FTSTicker::FDelegateHandle RequestProcessControllerStateHandle;
};
