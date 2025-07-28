// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "CommonPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "FrontendPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FRONTENDUI_API AFrontendPlayerController : public ACommonPlayerController
{
	GENERATED_BODY()

protected:
	//~Begin PlayerController
	virtual void OnPossess(APawn* InPawn) override;
	//~End
};
