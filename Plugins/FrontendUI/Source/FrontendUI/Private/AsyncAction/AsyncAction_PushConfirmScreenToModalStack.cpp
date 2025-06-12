// XiaoYao copyright.


#include "AsyncAction/AsyncAction_PushConfirmScreenToModalStack.h"

#include "Subsystem/FrontendUISubsystem.h"

UAsyncAction_PushConfirmScreenToModalStack* UAsyncAction_PushConfirmScreenToModalStack::PushConfirmScreen(
	const UObject* WorldContextObject, EConfirmScreenType InScreenType, const FText& InScreenTitle, const FText& InScreenMessage)
{
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			UAsyncAction_PushConfirmScreenToModalStack* PushConfirmScreen = NewObject<UAsyncAction_PushConfirmScreenToModalStack>();

			PushConfirmScreen->WorldContextObjectCached = World;
			PushConfirmScreen->ScreenTypeCached = InScreenType;
			PushConfirmScreen->ScreenTileCached = InScreenTitle;
			PushConfirmScreen->ScreenMessageCached = InScreenMessage;

			PushConfirmScreen->RegisterWithGameInstance(World);
			return PushConfirmScreen;
		}
	}
	return nullptr;
}

void UAsyncAction_PushConfirmScreenToModalStack::Activate()
{
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObjectCached.Get(), EGetWorldErrorMode::LogAndReturnNull))
		{
			if (UFrontendUISubsystem* FrontendUISubsystem = UFrontendUISubsystem::Get(World))
			{
				FrontendUISubsystem->PushConfirmScreenToModalStackAsync(ScreenTypeCached, ScreenTileCached, ScreenMessageCached,
					[this](EConfirmScreenButtonType ButtonType)
					{
						OnConfirmButtonClicked.Broadcast(ButtonType);

						SetReadyToDestroy();
					});
			}
		}
	}
}
