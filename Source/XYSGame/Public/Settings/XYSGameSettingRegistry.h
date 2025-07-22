// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "GameSettingRegistry.h"
#include "XYSGameSettingRegistry.generated.h"

/**
 * 
 */
UCLASS()
class XYSGAME_API UXYSGameSettingRegistry : public UGameSettingRegistry
{
	GENERATED_BODY()

public:
	UXYSGameSettingRegistry();

	static UXYSGameSettingRegistry* Get(UXYSLocalPlayer* InLocalPlayer);
	
	virtual void SaveChanges() override;

protected:
	virtual void OnInitialize(ULocalPlayer* InLocalPlayer) override;
	virtual bool IsFinishedInitializing() const override;

	UGameSettingCollection* InitializeVideoSettings(UXYSLocalPlayer* InLocalPlayer);
	void InitializeVideoSettings_FrameRates(UGameSettingCollection* Screen, UXYSLocalPlayer* InLocalPlayer);
	void AddPerformanceStatPage(UGameSettingCollection* Screen, UXYSLocalPlayer* InLocalPlayer);

	UGameSettingCollection* InitializeAudioSettings(UXYSLocalPlayer* InLocalPlayer);
	UGameSettingCollection* InitializeGameplaySettings(UXYSLocalPlayer* InLocalPlayer);

	UGameSettingCollection* InitializeMouseAndKeyboardSettings(UXYSLocalPlayer* InLocalPlayer);
	UGameSettingCollection* InitializeGamepadSettings(UXYSLocalPlayer* InLocalPlayer);

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> VideoSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> AudioSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> GameplaySettings;

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> MouseAndKeyboardSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> GamepadSettings;
};
