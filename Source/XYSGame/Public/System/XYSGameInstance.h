// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "CommonGameInstance.h"
#include "Engine/GameInstance.h"
#include "XYSGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class XYSGAME_API UXYSGameInstance : public UCommonGameInstance
{
	GENERATED_BODY()
	
public:
	UXYSGameInstance(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void Init() override;
};
