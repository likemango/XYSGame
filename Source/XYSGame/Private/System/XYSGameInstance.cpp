// XiaoYao copyright.


#include "System/XYSGameInstance.h"

#include "XYSGameplayTags.h"
#include "XYSLogChannels.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Subsystem/FrontendUISubsystem.h"

UXYSGameInstance::UXYSGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

int32 UXYSGameInstance::AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId)
{
	check(NewPlayer->PlayerController);
	int32 ReturnVal = Super::AddLocalPlayer(NewPlayer, UserId);
	if (ReturnVal != INDEX_NONE)
	{
		check (!PrimaryPlayer.IsValid())
		PrimaryPlayer = NewPlayer;
		
		GetSubsystem<UFrontendUISubsystem>()->NotifyPlayerAdded(NewPlayer);
	}
	
	return ReturnVal;
}

bool UXYSGameInstance::RemoveLocalPlayer(ULocalPlayer* ExistingPlayer)
{
	check(PrimaryPlayer == ExistingPlayer);
	
	GetSubsystem<UFrontendUISubsystem>()->NotifyPlayerRemoved(ExistingPlayer);
	return Super::RemoveLocalPlayer(ExistingPlayer);
}

void UXYSGameInstance::Init()
{
	Super::Init();

	// Register our custom init states
	UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);

	if (ensure(ComponentManager))
	{
		ComponentManager->RegisterInitState(XYSGameplayTags::InitState_Spawned, false, FGameplayTag());
		ComponentManager->RegisterInitState(XYSGameplayTags::InitState_DataAvailable, false, XYSGameplayTags::InitState_Spawned);
		ComponentManager->RegisterInitState(XYSGameplayTags::InitState_DataInitialized, false, XYSGameplayTags::InitState_DataAvailable);
		ComponentManager->RegisterInitState(XYSGameplayTags::InitState_GameplayReady, false, XYSGameplayTags::InitState_DataInitialized);
	}
}
