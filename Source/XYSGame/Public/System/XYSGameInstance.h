// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "XYSCommonGameInstance.h"
#include "Engine/GameInstance.h"
#include "XYSGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class XYSGAME_API UXYSGameInstance : public UXYSCommonGameInstance
{
	GENERATED_BODY()
	
public:
	UXYSGameInstance();

protected:
	virtual void Init() override;
};
