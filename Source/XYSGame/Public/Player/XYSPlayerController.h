// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XYSCommonPlayerController.h"
#include "Camera/XYSCameraAssistInterface.h"
#include "GameFramework/PlayerController.h"
#include "Teams/XYSTeamAgentInterface.h"
#include "XYSPlayerController.generated.h"

class UXYSSettingsShared;
class AXYSHUD;
class AXYSPlayerState;
class UXYSAbilitySystemComponent;
/**
 * The base player controller class used by this project.
 */
UCLASS(Config = Game, Meta = (ShortTooltip = "The base player controller class used by this project."))
class XYSGAME_API AXYSPlayerController : public AXYSCommonPlayerController, public IXYSTeamAgentInterface, public IXYSCameraAssistInterface
{
	GENERATED_BODY()

public:
	AXYSPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "XYS|PlayerController")
	AXYSPlayerState* GetXYSPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "XYS|PlayerController")
	UXYSAbilitySystemComponent* GetXYSAbilitySystemComponent() const;

	UFUNCTION(BlueprintCallable, Category = "XYS|PlayerController")
	AXYSHUD* GetXYSHUD() const;

	// Call to see if we should record a replay, subclasses could change this
	virtual bool ShouldRecordClientReplay();

	// Run a cheat command on the server.
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerCheat(const FString& Msg);

	// Run a cheat command on the server for all players.
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerCheatAll(const FString& Msg);

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~End of AActor interface

	//~AController interface
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void InitPlayerState() override;
	virtual void CleanupPlayerState() override;
	virtual void OnRep_PlayerState() override;
	//~End of AController interface

	//~APlayerController interface
	virtual void ReceivedPlayer() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetPlayer(UPlayer* InPlayer) override;
	virtual void AddCheats(bool bForce) override;
	virtual void UpdateForceFeedback(IInputInterface* InputInterface, const int32 ControllerId) override;
	virtual void
	UpdateHiddenComponents(const FVector& ViewLocation, TSet<FPrimitiveComponentId>& OutHiddenComponents) override;
	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	//~End of APlayerController interface

	//~IXYSCameraAssistInterface interface
	virtual void OnCameraPenetratingTarget() override;
	//~End of IXYSCameraAssistInterface interface

	//~IXYSTeamAgentInterface interface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnXYSTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//~End of IXYSTeamAgentInterface interface

	UFUNCTION(BlueprintCallable, Category = "XYS|Character")
	void SetIsAutoRunning(const bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "XYS|Character")
	bool GetIsAutoRunning() const;

private:
	UPROPERTY()
	FOnXYSTeamIndexChangedDelegate OnTeamChangedDelegate;

	UPROPERTY()
	TObjectPtr<APlayerState> LastSeenPlayerState;

private:
	UFUNCTION()
	void OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);

protected:
	// Called when the player state is set or cleared
	virtual void OnPlayerStateChanged();

private:
	void BroadcastOnPlayerStateChanged();

protected:
	//~APlayerController interface

	//~End of APlayerController interface

	void OnSettingsChanged(UXYSSettingsShared* Settings);

	void OnStartAutoRun();
	void OnEndAutoRun();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnStartAutoRun"))
	void K2_OnStartAutoRun();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnEndAutoRun"))
	void K2_OnEndAutoRun();

	bool bHideViewTargetPawnNextFrame = false;
};
