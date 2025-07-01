// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameState.h"
#include "Message/XYSVerbMessage.h"

#include "XYSGameState.generated.h"

/**
 * 
 */
UCLASS()
class XYSGAME_API AXYSGameState : public AModularGameStateBase
{
	GENERATED_BODY()

public:
	// Send a message that all clients will (probably) get
	// (use only for client notifications like eliminations, server join messages, etc... that can handle being lost)
	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable, Category = "Lyra|GameState")
	void MulticastMessageToClients(const FXYSVerbMessage Message);
	
	// Send a message that all clients will be guaranteed to get
	// (use only for client notifications that cannot handle being lost)
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Lyra|GameState")
	void MulticastReliableMessageToClients(const FXYSVerbMessage Message);
};
