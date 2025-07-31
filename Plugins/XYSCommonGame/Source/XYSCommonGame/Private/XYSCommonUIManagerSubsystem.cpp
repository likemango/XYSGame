// XiaoYao copyright.


#include "XYSCommonUIManagerSubsystem.h"

void UXYSCommonUIManagerSubsystem::NotifyPlayerAdded(UXYSCommonLocalPlayer* LocalPlayer)
{
}

void UXYSCommonUIManagerSubsystem::NotifyPlayerDestroyed(UXYSCommonLocalPlayer* LocalPlayer)
{
}

void UXYSCommonUIManagerSubsystem::NotifyPlayerRemoved(UXYSCommonLocalPlayer* LocalPlayer)
{
}


UCommonActivatableWidget* UXYSCommonUIManagerSubsystem::PushWidgetToLayerStack(ULocalPlayer* LocalPlayer, FGameplayTag LayerName,
	TSubclassOf<UCommonActivatableWidget> InActivatableWidgetClass)
{
	return nullptr;
}


void UXYSCommonUIManagerSubsystem::PushWidgetToLayerStackAsync(ULocalPlayer* LocalPlayer, FGameplayTag LayerName, bool bSuspendInputUntilComplete,
	TSoftClassPtr<UCommonActivatableWidget> WidgetClass)
{
}

void UXYSCommonUIManagerSubsystem::FindAndRemoveWidgetFromLayer(ULocalPlayer* LocalPlayer, UCommonActivatableWidget* InActivatableWidget)
{
}
