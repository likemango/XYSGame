// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "XYSGameUIManagerSubsystem.generated.h"

class UXYSCommonLocalPlayer;
/**
 * 
 */
UCLASS(Abstract)
class XYSCOMMONGAME_API UXYSGameUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void NotifyPlayerAdded(UXYSCommonLocalPlayer* LocalPlayer);
	virtual void NotifyPlayerDestroyed(UXYSCommonLocalPlayer* LocalPlayer);
	virtual void NotifyPlayerRemoved(UXYSCommonLocalPlayer* LocalPlayer);
};
