// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "XYSGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class XYSGAME_API UXYSGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UXYSGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual int32 AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId) override;
	virtual bool RemoveLocalPlayer(ULocalPlayer* ExistingPlayer) override;

protected:
	virtual void Init() override;

private:
	/** This is the primary player*/
	TWeakObjectPtr<ULocalPlayer> PrimaryPlayer;
};
