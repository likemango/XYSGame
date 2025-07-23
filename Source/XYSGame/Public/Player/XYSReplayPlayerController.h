// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "XYSPlayerController.h"
#include "XYSReplayPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class XYSGAME_API AXYSReplayPlayerController : public AXYSPlayerController
{
	GENERATED_BODY()
	
	virtual void Tick(float DeltaSeconds) override;
	virtual void SmoothTargetViewRotation(APawn* TargetPawn, float DeltaSeconds) override;
	virtual bool ShouldRecordClientReplay() override;

	// Callback for when the game state's RecorderPlayerState gets replicated during replay playback
	void RecorderPlayerStateUpdated(APlayerState* NewRecorderPlayerState);

	// Callback for when the followed player state changes pawn
	UFUNCTION()
	void OnPlayerStatePawnSet(APlayerState* ChangedPlayerState, APawn* NewPlayerPawn, APawn* OldPlayerPawn);

	// The player state we are currently following */
	UPROPERTY(Transient)
	TObjectPtr<APlayerState> FollowedPlayerState;
};
