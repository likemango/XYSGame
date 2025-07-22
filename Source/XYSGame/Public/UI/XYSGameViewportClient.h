// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "CommonGameViewportClient.h"
#include "XYSGameViewportClient.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class XYSGAME_API UXYSGameViewportClient : public UCommonGameViewportClient
{
	GENERATED_BODY()

public:
	UXYSGameViewportClient();

	virtual void Init(struct FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice = true) override;
};
