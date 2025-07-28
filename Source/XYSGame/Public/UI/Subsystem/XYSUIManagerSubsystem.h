// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "GameUIManagerSubsystem.h"
#include "XYSUIManagerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class XYSGAME_API UXYSUIManagerSubsystem : public UGameUIManagerSubsystem
{
	GENERATED_BODY()
	
public:
	UXYSUIManagerSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	bool Tick(float DeltaTime);
	void SyncRootLayoutVisibilityToShowHUD();

	FTSTicker::FDelegateHandle TickHandle;
};
