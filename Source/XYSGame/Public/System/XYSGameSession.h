// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "XYSGameSession.generated.h"

UCLASS(Config=Game)
class XYSGAME_API AXYSGameSession : public AGameSession
{
	GENERATED_BODY()

public:
	AXYSGameSession(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	/** Override to disable the default behavior */
	virtual bool ProcessAutoLogin() override;

	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;
};
