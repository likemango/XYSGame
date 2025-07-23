// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "GameFeatures/GameFeatureAction_AddInputContextMapping.h"
#include "Components/PawnComponent.h"
#include "Input/XYSInputConfig.h"
#include "XYSHeroComponent.generated.h"

struct FInputActionValue;
/**
 * Component that sets up input and camera handling for player controlled pawns (or bots that simulate players).
 * This depends on a PawnExtensionComponent to coordinate initialization.
 */
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class XYSGAME_API UXYSHeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	UXYSHeroComponent(const FObjectInitializer& ObjectInitializer);

	/** The name of this component-implemented feature */
	static const FName NAME_ActorFeatureName;
	static const FName NAME_BindInputsNow;
	
	//~ Begin IGameFrameworkInitStateInterface interface
	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual void CheckDefaultInitialization() override;
	//~End

	/** Returns the hero component if one exists on the specified actor. */
	UFUNCTION(BlueprintPure, Category = "Hero")
	static UXYSHeroComponent* FindHeroComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UXYSHeroComponent>() : nullptr); }

	/** Adds mode-specific input config */
	void AddAdditionalInputConfig(const UXYSInputConfig* InputConfig);
	/** Removes a mode-specific input config if it has been added */
	void RemoveAdditionalInputConfig(const UXYSInputConfig* InputConfig);
	/** True if this is controlled by a real player and has progressed far enough in initialization where additional input bindings can be added */
	bool IsReadyToBindInputs() const;

protected:
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);

	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);
	/*void Input_Crouch(const FInputActionValue& InputActionValue);*/

	UPROPERTY(EditDefaultsOnly)
	TArray<FInputMappingContextAndPriority> DefaultInputMappings;

	/** True when player input bindings have been applied, will never be true for non - players */
	bool bReadyToBindInputs;

private:
	TArray<uint32> BaseBindHandles;
	TArray<uint32> AdditionalBindHandles;
		
};
