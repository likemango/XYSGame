// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "XYSCommonGameInstance.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class XYSCOMMONGAME_API UXYSCommonGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UXYSCommonGameInstance();
	
	virtual int32 AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId) override;
	virtual bool RemoveLocalPlayer(ULocalPlayer* ExistingPlayer) override;
	virtual void Init() override;
	virtual void ReturnToMainMenu() override;

private:
	/** This is the primary player*/
	TWeakObjectPtr<ULocalPlayer> PrimaryPlayer;
};
