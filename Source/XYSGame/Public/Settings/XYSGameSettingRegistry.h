// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "GameSettingRegistry.h"
#include "XYSGameSettingRegistry.generated.h"

class UXYSLocalPlayer;

DECLARE_LOG_CATEGORY_EXTERN(LogXYSGameSettingRegistry, Log, Log);

#define GET_SHARED_SETTINGS_FUNCTION_PATH(FunctionOrPropertyName)							\
MakeShared<FGameSettingDataSourceDynamic>(TArray<FString>({								\
GET_FUNCTION_NAME_STRING_CHECKED(UXYSLocalPlayer, GetSharedSettings),				\
GET_FUNCTION_NAME_STRING_CHECKED(UXYSSettingsShared, FunctionOrPropertyName)		\
}))

#define GET_LOCAL_SETTINGS_FUNCTION_PATH(FunctionOrPropertyName)							\
MakeShared<FGameSettingDataSourceDynamic>(TArray<FString>({								\
GET_FUNCTION_NAME_STRING_CHECKED(UXYSLocalPlayer, GetLocalSettings),				\
GET_FUNCTION_NAME_STRING_CHECKED(UXYSSettingsLocal, FunctionOrPropertyName)		\
}))

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
