// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "XYSPlayerSpawningManagerComponent.generated.h"


class AXYSPlayerStart;

UCLASS()
class XYSGAME_API UXYSPlayerSpawningManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()
public:
	 UXYSPlayerSpawningManagerComponent(const FObjectInitializer& ObjectInitializer);

	/** UActorComponent */
	 virtual void InitializeComponent() override;
	 virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	/** ~UActorComponent */

protected:
	// Utility
	 APlayerStart* GetFirstRandomUnoccupiedPlayerStart(AController* Controller, const TArray<AXYSPlayerStart*>& FoundStartPoints) const;
	
	virtual AActor* OnChoosePlayerStart(AController* Player, TArray<AXYSPlayerStart*>& PlayerStarts) { return nullptr; }
	virtual void OnFinishRestartPlayer(AController* Player, const FRotator& StartRotation) { }

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName=OnFinishRestartPlayer))
	 void K2_OnFinishRestartPlayer(AController* Player, const FRotator& StartRotation);

private:

	/** We proxy these calls from AXYSGameMode, to this component so that each experience can more easily customize the respawn system they want. */
	 AActor* ChoosePlayerStart(AController* Player);
	 bool ControllerCanRestart(AController* Player);
	 void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation);
	friend class AXYSGameMode;
	/** ~AXYSGameMode */

	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<AXYSPlayerStart>> CachedPlayerStarts;

private:
	 void OnLevelAdded(ULevel* InLevel, UWorld* InWorld);
	 void HandleOnActorSpawned(AActor* SpawnedActor);

#if WITH_EDITOR
	 APlayerStart* FindPlayFromHereStart(AController* Player);
#endif
};
