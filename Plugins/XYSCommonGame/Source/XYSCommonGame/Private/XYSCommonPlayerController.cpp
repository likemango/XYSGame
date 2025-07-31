// XiaoYao copyright.


#include "XYSCommonPlayerController.h"
#include "XYSCommonLocalPlayer.h"

class APawn;

AXYSCommonPlayerController::AXYSCommonPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AXYSCommonPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
	
	if (UXYSCommonLocalPlayer* LocalPlayer = Cast<UXYSCommonLocalPlayer>(Player))
	{
		LocalPlayer->OnPlayerControllerSet.Broadcast(LocalPlayer, this);

		if (PlayerState)
		{
			LocalPlayer->OnPlayerStateSet.Broadcast(LocalPlayer, PlayerState);
		}
	}
}

void AXYSCommonPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (UXYSCommonLocalPlayer* LocalPlayer = Cast<UXYSCommonLocalPlayer>(Player))
	{
		LocalPlayer->OnPlayerPawnSet.Broadcast(LocalPlayer, InPawn);
	}
}

void AXYSCommonPlayerController::OnPossess(APawn* APawn)
{
	Super::OnPossess(APawn);
	
	if (UXYSCommonLocalPlayer* LocalPlayer = Cast<UXYSCommonLocalPlayer>(Player))
	{
		LocalPlayer->OnPlayerPawnSet.Broadcast(LocalPlayer, APawn);
	}
}

void AXYSCommonPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	if (UXYSCommonLocalPlayer* LocalPlayer = Cast<UXYSCommonLocalPlayer>(Player))
	{
		LocalPlayer->OnPlayerPawnSet.Broadcast(LocalPlayer, nullptr);
	}
}

void AXYSCommonPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (PlayerState)
	{
		if (UXYSCommonLocalPlayer* LocalPlayer = Cast<UXYSCommonLocalPlayer>(Player))
		{
			LocalPlayer->OnPlayerStateSet.Broadcast(LocalPlayer, PlayerState);
		}
	}
}
