// XiaoYao copyright.


#include "AsyncAction/AsyncAction_PushSoftWidgetClassToStack.h"

#include "Subsystem/FrontendUISubsystem.h"
#include "Widgets/Widget_ActivatableBase.h"

UAsyncAction_PushSoftWidgetClassToStack* UAsyncAction_PushSoftWidgetClassToStack::PushSoftWidget(
	const UObject* WorldContextObject, APlayerController* OwningPlayerController, UPARAM(meta=(Categories="Frontend.WidgetStack")) FGameplayTag StackTag,
	TSoftClassPtr<UWidget_ActivatableBase> SoftWidgetClass, bool bFocusOnNewlyPushedWidget)
{
	checkf(!SoftWidgetClass.IsNull(), TEXT("Widget class in PushSoftWidgetClassToStack is null!"))
	
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			UAsyncAction_PushSoftWidgetClassToStack* CreatedNode = NewObject<UAsyncAction_PushSoftWidgetClassToStack>();

			CreatedNode->WorldContextObjectCached = World;
			CreatedNode->OwningPlayerControllerCached = OwningPlayerController;
			CreatedNode->bFocusOnNewlyPushedWidgetCached = bFocusOnNewlyPushedWidget;
			CreatedNode->SoftWidgetClassCached = SoftWidgetClass;
			CreatedNode->StackTagCached = StackTag;

			CreatedNode->RegisterWithGameInstance(World);
			return CreatedNode;
		}
	}
	return nullptr;
}

void UAsyncAction_PushSoftWidgetClassToStack::Activate()
{
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObjectCached.Get(), EGetWorldErrorMode::Assert))
		{
			UFrontendUISubsystem* FrontendUISubsystem = UFrontendUISubsystem::Get(World);

			FrontendUISubsystem->PushSoftWidgetClassToStackAsync(StackTagCached, SoftWidgetClassCached,
				[this](EAsyncPushWidgetState State, UWidget_ActivatableBase* CreatedWidget)
			{
				switch (State)
				{
					case EAsyncPushWidgetState::OnCreatedBeforePush:
						CreatedWidget->SetOwningPlayer(OwningPlayerControllerCached.Get());
						CreatedBeforePush.Broadcast(CreatedWidget);
						break;
					case EAsyncPushWidgetState::AfterPush:
						if (bFocusOnNewlyPushedWidgetCached)
						{
							if (UWidget* FocusWidget = CreatedWidget->GetDesiredFocusTarget())
							{
								FocusWidget->SetFocus();
							}
							AfterPush.Broadcast(CreatedWidget);
							SetReadyToDestroy();
						break;
						}
					default:
						break;
				}
			});
		}
	}
}
