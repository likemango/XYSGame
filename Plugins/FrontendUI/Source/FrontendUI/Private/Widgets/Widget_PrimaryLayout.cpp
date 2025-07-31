// XiaoYao copyright.


#include "Widgets/Widget_PrimaryLayout.h"

#include "Debug.h"
#include "GameplayTagContainer.h"
#include "XYSCommonLocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/FrontendUISubsystem.h"

UWidget_PrimaryLayout* UWidget_PrimaryLayout::GetPrimaryGameLayoutForPrimaryPlayer(const UObject* WorldContextObject)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	APlayerController* PlayerController = GameInstance->GetPrimaryPlayerController(false);
	return GetPrimaryGameLayout(PlayerController);
}

UWidget_PrimaryLayout* UWidget_PrimaryLayout::GetPrimaryGameLayout(APlayerController* PlayerController)
{
	return PlayerController ? GetPrimaryGameLayout(Cast<UXYSCommonLocalPlayer>(PlayerController->Player)) : nullptr;
}

UWidget_PrimaryLayout* UWidget_PrimaryLayout::GetPrimaryGameLayout(ULocalPlayer* LocalPlayer)
{
	if (LocalPlayer)
	{
		const UXYSCommonLocalPlayer* CommonLocalPlayer = CastChecked<UXYSCommonLocalPlayer>(LocalPlayer);
		if (const UGameInstance* GameInstance = CommonLocalPlayer->GetGameInstance())
		{
			if (UFrontendUISubsystem* UISubsystem = UFrontendUISubsystem::Get(GameInstance))
			{
				return UISubsystem->GetRootLayout(CommonLocalPlayer);
			}
		}
	}

	return nullptr;
}

UCommonActivatableWidgetContainerBase* UWidget_PrimaryLayout::FindWidgetStackByTag(const FGameplayTag& InTag) const
{
	ensureAlwaysMsgf(Layers.Contains(InTag), TEXT("Can not find the widget stack by the tag %s"), *InTag.ToString());
	if (InTag.IsValid())
	{
		return Layers.FindRef(InTag);
	}
	return nullptr;
}

void UWidget_PrimaryLayout::FindAndRemoveWidgetFromLayer(UCommonActivatableWidget* ActivatableWidget)
{
	// We're not sure what layer the widget is on so go searching.
	for (const auto& LayerKVP : Layers)
	{
		LayerKVP.Value->RemoveWidget(*ActivatableWidget);
	}
}

UCommonActivatableWidgetContainerBase* UWidget_PrimaryLayout::GetLayerWidget(FGameplayTag LayerName) const
{
	return Layers.FindRef(LayerName);
}

void UWidget_PrimaryLayout::RegisterLayer(FGameplayTag LayerTag, UCommonActivatableWidgetContainerBase* LayerWidget)
{
	check(LayerWidget);

	if (!IsDesignTime())
	{
		LayerWidget->OnTransitioningChanged.AddUObject(this, &ThisClass::OnWidgetStackTransitioning);
		// TODO: Consider allowing a transition duration, we currently set it to 0, because if it's not 0, the
		//       transition effect will cause focus to not transition properly to the new widgets when using
		//       gamepad always.
		// LayerWidget->SetTransitionDuration(0.0);

		Layers.Add(LayerTag, LayerWidget);
	}
}

void UWidget_PrimaryLayout::OnWidgetStackTransitioning(UCommonActivatableWidgetContainerBase* Widget,bool bIsTransitioning)
{
	if (bIsTransitioning)
	{
		const FName SuspendToken = UXYSCommonUIExtensions::SuspendInputForPlayer(GetOwningLocalPlayer(), TEXT("GlobalStackTransion"));
		SuspendInputTokens.Add(SuspendToken);
	}
	else
	{
		if (ensure(SuspendInputTokens.Num() > 0))
		{
			const FName SuspendToken = SuspendInputTokens.Pop();
			UXYSCommonUIExtensions::ResumeInputForPlayer(GetOwningLocalPlayer(), SuspendToken);
		}
	}
}
