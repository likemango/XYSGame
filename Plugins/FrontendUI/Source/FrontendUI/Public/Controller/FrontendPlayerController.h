// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "XYSCommonPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "FrontendPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FRONTENDUI_API AFrontendPlayerController : public AXYSCommonPlayerController
{
	GENERATED_BODY()

protected:
	//~Begin PlayerController
	virtual void OnPossess(APawn* InPawn) override;
	//~End
};
