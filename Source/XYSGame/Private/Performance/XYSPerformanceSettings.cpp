// XiaoYao copyright.


#include "Performance/XYSPerformanceSettings.h"
#include "Performance/XYSPerformanceStatTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(XYSPerformanceSettings)

//////////////////////////////////////////////////////////////////////

UXYSPlatformSpecificRenderingSettings::UXYSPlatformSpecificRenderingSettings()
{
	MobileFrameRateLimits.Append({ 20, 30, 45, 60, 90, 120 });
}

const UXYSPlatformSpecificRenderingSettings* UXYSPlatformSpecificRenderingSettings::Get()
{
	UXYSPlatformSpecificRenderingSettings* Result = UPlatformSettingsManager::Get().GetSettingsForPlatform<ThisClass>();
	check(Result);
	return Result;
}

//////////////////////////////////////////////////////////////////////

UXYSPerformanceSettings::UXYSPerformanceSettings()
{
	PerPlatformSettings.Initialize(UXYSPlatformSpecificRenderingSettings::StaticClass());

	CategoryName = TEXT("Game");

	DesktopFrameRateLimits.Append({ 30, 60, 120, 144, 160, 165, 180, 200, 240, 360 });

	// Default to all stats are allowed
	FXYSPerformanceStatGroup& StatGroup = UserFacingPerformanceStats.AddDefaulted_GetRef();
	for (EXYSDisplayablePerformanceStat PerfStat : TEnumRange<EXYSDisplayablePerformanceStat>())
	{
		StatGroup.AllowedStats.Add(PerfStat);
	}
}

