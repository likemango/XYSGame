// XiaoYao copyright.


#include "System/XYSGameSession.h"


AXYSGameSession::AXYSGameSession(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool AXYSGameSession::ProcessAutoLogin()
{
	// This is actually handled in XYSGameMode::TryDedicatedServerLogin
	return true;
}

void AXYSGameSession::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
}

void AXYSGameSession::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
}
