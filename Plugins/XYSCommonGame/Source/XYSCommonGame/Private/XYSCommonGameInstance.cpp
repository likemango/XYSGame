// XiaoYao copyright.


#include "XYSCommonGameInstance.h"

#include "XYSCommonLocalPlayer.h"
#include "XYSCommonUIManagerSubsystem.h"

UXYSCommonGameInstance::UXYSCommonGameInstance()
{
	
}

int32 UXYSCommonGameInstance::AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId)
{
	int32 ReturnVal = Super::AddLocalPlayer(NewPlayer, UserId);
	if (ReturnVal != INDEX_NONE)
	{
		check(!PrimaryPlayer.IsValid())
		UE_LOG(LogTemp, Log, TEXT("AddLocalPlayer: Set %s to Primary Player"), *NewPlayer->GetName());
		PrimaryPlayer = NewPlayer;
		
		GetSubsystem<UXYSCommonUIManagerSubsystem>()->NotifyPlayerAdded(Cast<UXYSCommonLocalPlayer>(NewPlayer));
	}
	
	return ReturnVal;
}

bool UXYSCommonGameInstance::RemoveLocalPlayer(ULocalPlayer* ExistingPlayer)
{
	check(PrimaryPlayer == ExistingPlayer);
	
	GetSubsystem<UXYSCommonUIManagerSubsystem>()->NotifyPlayerRemoved(Cast<UXYSCommonLocalPlayer>(ExistingPlayer));
	return Super::RemoveLocalPlayer(ExistingPlayer);
}

void UXYSCommonGameInstance::Init()
{
	Super::Init();
}

void UXYSCommonGameInstance::ReturnToMainMenu()
{
	Super::ReturnToMainMenu();
}
