// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "XYSCommonUIManagerSubsystem.generated.h"

class UCommonActivatableWidget;
class UXYSCommonLocalPlayer;
/**
 * 
 */
UCLASS(Abstract)
class XYSCOMMONGAME_API UXYSCommonUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void NotifyPlayerAdded(UXYSCommonLocalPlayer* LocalPlayer);
	virtual void NotifyPlayerDestroyed(UXYSCommonLocalPlayer* LocalPlayer);
	virtual void NotifyPlayerRemoved(UXYSCommonLocalPlayer* LocalPlayer);

	virtual UCommonActivatableWidget* PushWidgetToLayerStack(ULocalPlayer* LocalPlayer, FGameplayTag LayerName, TSubclassOf<UCommonActivatableWidget> InActivatableWidgetClass);
	virtual void PushWidgetToLayerStackAsync(ULocalPlayer* LocalPlayer, FGameplayTag LayerName, bool bSuspendInputUntilComplete, TSoftClassPtr<UCommonActivatableWidget> WidgetClass);
	virtual void FindAndRemoveWidgetFromLayer(ULocalPlayer* LocalPlayer, UCommonActivatableWidget* InActivatableWidget);
	
};