// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameMode.h"
#include "XYSGameMode.generated.h"

class UXYSPawnData;
/**
 * 
 */
UCLASS()
class XYSGAME_API AXYSGameMode : public AModularGameModeBase
{
	GENERATED_BODY()

public:
	AXYSGameMode(const FObjectInitializer& ObjectInitializer);

	// ~Begin AGameModeBase interface
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
	// ~End

	// Restart (respawn) the specified player or bot next frame
	// - If bForceReset is true, the controller will be reset this frame (abandoning the currently possessed pawn, if any)
	UFUNCTION(BlueprintCallable)
	void RequestPlayerRestartNextFrame(AController* Controller, bool bForceReset = false);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PawnData")
	TObjectPtr<UXYSPawnData> PawnData;
	
};
