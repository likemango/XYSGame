// XiaoYao copyright.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "XYSVerbMessageHelper.generated.h"

struct FXYSVerbMessage;
struct FGameplayCueParameters;

class APlayerController;
class APlayerState;
class UObject;
struct FFrame;

/**
 * 
 */
UCLASS()
class XYSGAME_API UXYSVerbMessageHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "XYS")
	static APlayerState* GetPlayerStateFromObject(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "XYS")
	static APlayerController* GetPlayerControllerFromObject(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "XYS")
	static FGameplayCueParameters VerbMessageToCueParameters(const FXYSVerbMessage& Message);

	UFUNCTION(BlueprintCallable, Category = "XYS")
	static FXYSVerbMessage CueParametersToVerbMessage(const FGameplayCueParameters& Params);
};
