// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"

#include "XYSCommonPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class XYSCOMMONGAME_API AXYSCommonPlayerController : public AModularPlayerController
{
	GENERATED_BODY()

public:
	AXYSCommonPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void ReceivedPlayer() override;
	virtual void SetPawn(APawn* InPawn) override;
	virtual void OnPossess(class APawn* APawn) override;
	virtual void OnUnPossess() override;

protected:
	virtual void OnRep_PlayerState() override;
};
