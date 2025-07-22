// XiaoYao copyright.


#include "Settings/XYSGameSettingRegistry.h"

#include "GameSettingCollection.h"
#include "Player/XYSLocalPlayer.h"
#include "Settings/XYSSettingsLocal.h"
#include "Settings/XYSSettingsShared.h"

UXYSGameSettingRegistry::UXYSGameSettingRegistry()
{
}

UXYSGameSettingRegistry* UXYSGameSettingRegistry::Get(UXYSLocalPlayer* InLocalPlayer)
{
	UXYSGameSettingRegistry* Registry = FindObject<UXYSGameSettingRegistry>(InLocalPlayer, TEXT("XYSGameSettingRegistry"), true);
	if (Registry == nullptr)
	{
		Registry = NewObject<UXYSGameSettingRegistry>(InLocalPlayer, TEXT("XYSGameSettingRegistry"));
		Registry->Initialize(InLocalPlayer);
	}

	return Registry;
}

bool UXYSGameSettingRegistry::IsFinishedInitializing() const
{
	if (Super::IsFinishedInitializing())
	{
		if (UXYSLocalPlayer* LocalPlayer = Cast<UXYSLocalPlayer>(OwningLocalPlayer))
		{
			if (LocalPlayer->GetSharedSettings() == nullptr)
			{
				return false;
			}
		}

		return true;
	}

	return false;
}

void UXYSGameSettingRegistry::OnInitialize(ULocalPlayer* InLocalPlayer)
{
	UXYSLocalPlayer* XYSLocalPlayer = Cast<UXYSLocalPlayer>(InLocalPlayer);

	VideoSettings = InitializeVideoSettings(XYSLocalPlayer);
	InitializeVideoSettings_FrameRates(VideoSettings, XYSLocalPlayer);
	RegisterSetting(VideoSettings);

	AudioSettings = InitializeAudioSettings(XYSLocalPlayer);
	RegisterSetting(AudioSettings);

	GameplaySettings = InitializeGameplaySettings(XYSLocalPlayer);
	RegisterSetting(GameplaySettings);

	MouseAndKeyboardSettings = InitializeMouseAndKeyboardSettings(XYSLocalPlayer);
	RegisterSetting(MouseAndKeyboardSettings);

	GamepadSettings = InitializeGamepadSettings(XYSLocalPlayer);
	RegisterSetting(GamepadSettings);
}

void UXYSGameSettingRegistry::SaveChanges()
{
	Super::SaveChanges();
	
	if (UXYSLocalPlayer* LocalPlayer = Cast<UXYSLocalPlayer>(OwningLocalPlayer))
	{
		// Game user settings need to be applied to handle things like resolution, this saves indirectly
		LocalPlayer->GetLocalSettings()->ApplySettings(false);
		
		LocalPlayer->GetSharedSettings()->ApplySettings();
		LocalPlayer->GetSharedSettings()->SaveSettings();
	}
}