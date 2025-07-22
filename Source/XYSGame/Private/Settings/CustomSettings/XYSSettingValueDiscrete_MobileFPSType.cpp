// XiaoYao copyright.


#include "Settings/CustomSettings/XYSSettingValueDiscrete_MobileFPSType.h"

#include "Performance/XYSPerformanceSettings.h"
#include "Settings/XYSSettingsLocal.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(XYSSettingValueDiscrete_MobileFPSType)

#define LOCTEXT_NAMESPACE "XYSSettings"

UXYSSettingValueDiscrete_MobileFPSType::UXYSSettingValueDiscrete_MobileFPSType()
{
}

void UXYSSettingValueDiscrete_MobileFPSType::OnInitialized()
{
	Super::OnInitialized();

	const UXYSPlatformSpecificRenderingSettings* PlatformSettings = UXYSPlatformSpecificRenderingSettings::Get();
	const UXYSSettingsLocal* UserSettings = UXYSSettingsLocal::Get();

	for (int32 TestLimit : PlatformSettings->MobileFrameRateLimits)
	{
		if (UXYSSettingsLocal::IsSupportedMobileFramePace(TestLimit))
		{
			FPSOptions.Add(TestLimit, MakeLimitString(TestLimit));
		}
	}

	const int32 FirstFrameRateWithQualityLimit = UserSettings->GetFirstFrameRateWithQualityLimit();
	if (FirstFrameRateWithQualityLimit > 0)
	{
		SetWarningRichText(FText::Format(LOCTEXT("MobileFPSType_Note", "<strong>Note: Changing the framerate setting to {0} or higher might lower your Quality Presets.</>"), MakeLimitString(FirstFrameRateWithQualityLimit)));
	}
}

int32 UXYSSettingValueDiscrete_MobileFPSType::GetDefaultFPS() const
{
	return UXYSSettingsLocal::GetDefaultMobileFrameRate();
}

FText UXYSSettingValueDiscrete_MobileFPSType::MakeLimitString(int32 Number)
{
	return FText::Format(LOCTEXT("MobileFrameRateOption", "{0} FPS"), FText::AsNumber(Number));
}

void UXYSSettingValueDiscrete_MobileFPSType::StoreInitial()
{
	InitialValue = GetValue();
}

void UXYSSettingValueDiscrete_MobileFPSType::ResetToDefault()
{
	SetValue(GetDefaultFPS(), EGameSettingChangeReason::ResetToDefault);
}

void UXYSSettingValueDiscrete_MobileFPSType::RestoreToInitial()
{
	SetValue(InitialValue, EGameSettingChangeReason::RestoreToInitial);
}

void UXYSSettingValueDiscrete_MobileFPSType::SetDiscreteOptionByIndex(int32 Index)
{
	TArray<int32> FPSOptionsModes;
	FPSOptions.GenerateKeyArray(FPSOptionsModes);

	int32 NewMode = FPSOptionsModes.IsValidIndex(Index) ? FPSOptionsModes[Index] : GetDefaultFPS();

	SetValue(NewMode, EGameSettingChangeReason::Change);
}

int32 UXYSSettingValueDiscrete_MobileFPSType::GetDiscreteOptionIndex() const
{
	TArray<int32> FPSOptionsModes;
	FPSOptions.GenerateKeyArray(FPSOptionsModes);
	return FPSOptionsModes.IndexOfByKey(GetValue());
}

TArray<FText> UXYSSettingValueDiscrete_MobileFPSType::GetDiscreteOptions() const
{
	TArray<FText> Options;
	FPSOptions.GenerateValueArray(Options);

	return Options;
}

int32 UXYSSettingValueDiscrete_MobileFPSType::GetValue() const
{
	return UXYSSettingsLocal::Get()->GetDesiredMobileFrameRateLimit();
}

void UXYSSettingValueDiscrete_MobileFPSType::SetValue(int32 NewLimitFPS, EGameSettingChangeReason InReason)
{
	UXYSSettingsLocal::Get()->SetDesiredMobileFrameRateLimit(NewLimitFPS);

	NotifySettingChanged(InReason);
}

#undef LOCTEXT_NAMESPACE

