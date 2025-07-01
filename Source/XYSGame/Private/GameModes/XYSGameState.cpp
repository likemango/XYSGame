// XiaoYao copyright.


#include "GameModes/XYSGameState.h"

#include "GameFramework/GameplayMessageSubsystem.h"

void AXYSGameState::MulticastMessageToClients_Implementation(const FXYSVerbMessage Message)
{
	if (GetNetMode() == NM_Client)
	{
		UGameplayMessageSubsystem::Get(this).BroadcastMessage(Message.Verb, Message);
	}
}

void AXYSGameState::MulticastReliableMessageToClients_Implementation(const FXYSVerbMessage Message)
{
	MulticastMessageToClients_Implementation(Message);
}
