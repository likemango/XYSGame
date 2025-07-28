// XiaoYao copyright.


#include "XYSEditorEngine.h"

#include "Development/XYSDeveloperSettings.h"
#include "Framework/Notifications/NotificationManager.h"
#include "GameModes/XYSWorldSettings.h"
#include "Settings/ContentBrowserSettings.h"
#include "Widgets/Notifications/SNotificationList.h"

UXYSEditorEngine::UXYSEditorEngine(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UXYSEditorEngine::Init(IEngineLoop* InEngineLoop)
{
	Super::Init(InEngineLoop);
}

void UXYSEditorEngine::Start()
{
	Super::Start();
}

void UXYSEditorEngine::Tick(float DeltaSeconds, bool bIdleMode)
{
	Super::Tick(DeltaSeconds, bIdleMode);

	FirstTickSetup();
}

void UXYSEditorEngine::FirstTickSetup()
{
	if (bFirstTickSetup)
	{
		return;
	}

	bFirstTickSetup = true;

	// Force show plugin content on load.
	GetMutableDefault<UContentBrowserSettings>()->SetDisplayPluginFolders(true);
}

FGameInstancePIEResult UXYSEditorEngine::PreCreatePIEInstances(const bool bAnyBlueprintErrors,
	const bool bStartInSpectatorMode, const float PIEStartTime, const bool bSupportsOnlinePIE,
	int32& InNumOnlinePIEInstances)
{
	if (const AXYSWorldSettings* XYSWorldSettings = Cast<AXYSWorldSettings>(EditorWorld->GetWorldSettings()))
	{
		if (XYSWorldSettings->ForceStandaloneNetMode)
		{
			EPlayNetMode OutPlayNetMode;
			PlaySessionRequest->EditorPlaySettings->GetPlayNetMode(OutPlayNetMode);
			if (OutPlayNetMode != PIE_Standalone)
			{
				PlaySessionRequest->EditorPlaySettings->SetPlayNetMode(PIE_Standalone);

				FNotificationInfo Info(FText::FromString(TEXT("Forcing NetMode: Standalone for the Frontend")));
				Info.ExpireDuration = 2.0f;
				FSlateNotificationManager::Get().AddNotification(Info);
			}
		}
	}

	//@TODO: Should add delegates that a *non-editor* module could bind to for PIE start/stop instead of poking directly
	GetDefault<UXYSDeveloperSettings>()->OnPlayInEditorStarted();
	// GetDefault<UXYSPlatformEmulationSettings>()->OnPlayInEditorStarted();

	FGameInstancePIEResult Result = Super::PreCreatePIEServerInstance(bAnyBlueprintErrors, bStartInSpectatorMode, PIEStartTime, bSupportsOnlinePIE, InNumOnlinePIEInstances);

	return Result;
}

