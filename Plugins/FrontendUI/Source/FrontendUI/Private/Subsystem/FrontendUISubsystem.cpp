// XiaoYao copyright.


#include "Subsystem/FrontendUISubsystem.h"
#include "Engine/LocalPlayer.h"
#include "Debug.h"
#include "FrontendGameplayTags.h"
#include "XYSCommonLocalPlayer.h"
#include "Engine/AssetManager.h"
#include "FunctionLibrary/FrontendBlueprintFunctionLibrary.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "Widgets/Widget_ConfirmScreen.h"
#include "Widgets/Widget_PrimaryLayout.h"

UFrontendUISubsystem* UFrontendUISubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
		return UGameInstance::GetSubsystem<UFrontendUISubsystem>(World->GetGameInstance());
	}
	return nullptr;
}

void UFrontendUISubsystem::RegisterAndCallPrimaryLayoutCreated(FSimpleMulticastDelegate::FDelegate Delegate)
{
	if (!OnPrimaryLayoutAdded.IsBoundToObject(Delegate.GetUObject()))
	{
		OnPrimaryLayoutAdded.Add(Delegate);
	}
	
	if (RootViewportLayout.RootLayout)
	{
		Delegate.Execute();
	}
}

bool UFrontendUISubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (Outer)
	{
		if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
		{
			TArray<UClass*> FoundClasses;
			GetDerivedClasses(GetClass(), FoundClasses);
			return FoundClasses.IsEmpty();
		}
	}
	return false;
}


void UFrontendUISubsystem::PushSoftWidgetClassToStackAsync(const FGameplayTag& InTag,
                                                           TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass,
                                                           TFunction<void(EAsyncWidgetLayerState, UWidget_ActivatableBase*)> AsyncPushWidgetCallback) 
{
	UWidget_PrimaryLayout* PrimaryLayout = RootViewportLayout.RootLayout;
	ensure(PrimaryLayout);
	
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(InSoftWidgetClass.ToSoftObjectPath(), FStreamableDelegate::CreateLambda(
		[InSoftWidgetClass, this, InTag, AsyncPushWidgetCallback, PrimaryLayout]()
		{
			UClass* LoadedClass = InSoftWidgetClass.Get();
			check(PrimaryLayout && LoadedClass);
			UCommonActivatableWidgetContainerBase* ContainBase = PrimaryLayout->FindWidgetStackByTag(InTag);
			check(ContainBase);
			UWidget_ActivatableBase* CreatedPushedWidget = ContainBase->AddWidget<UWidget_ActivatableBase>(LoadedClass, [AsyncPushWidgetCallback](UWidget_ActivatableBase& CreatedWidget)
			{
				AsyncPushWidgetCallback(EAsyncWidgetLayerState::Initialize, &CreatedWidget);
			});
			AsyncPushWidgetCallback(EAsyncWidgetLayerState::AfterPush, CreatedPushedWidget);
		}
	));
}

void UFrontendUISubsystem::PushConfirmScreenToModalStackAsync(EConfirmScreenType InScreenType,const FText& InScreenTitle, const FText& InScreenMessage,
	TFunction<void(EConfirmScreenButtonType)> ButtonClickedCallback)
{
	UWidget_ConfirmScreen::FConfirmScreenInfoStruct ScreenInfo;

	//1.根据ConfirmScreen的类型需求，构建所需数据结构
	switch (InScreenType) {
	case EConfirmScreenType::OK:
		ScreenInfo = UWidget_ConfirmScreen::CreateOkScreen(InScreenTitle, InScreenMessage);
		break;
	case EConfirmScreenType::YesNo:
		ScreenInfo = UWidget_ConfirmScreen::CreateYesNoScreen(InScreenTitle, InScreenMessage);
		break;
	case EConfirmScreenType::OKCancel:
		ScreenInfo = UWidget_ConfirmScreen::CreateOkCancelScreen(InScreenTitle, InScreenMessage);
		break;
	case EConfirmScreenType::Unknown:
		break;
	}

	//2.Load/Create 并且在push前，根据1中生成的数据结构，初始化ConfirmScreen的结构
	TSoftClassPtr<UWidget_ActivatableBase> ConfirmScreenWidgetClass = UFrontendBlueprintFunctionLibrary::GetFrontendSoftWidgetClassByTag(FrontendGameplayTags::Frontend_Widget_ConfirmScreen);
	PushSoftWidgetClassToStackAsync(FrontendGameplayTags::Frontend_WidgetStack_Modal, ConfirmScreenWidgetClass,
		[ScreenInfo, ButtonClickedCallback](EAsyncWidgetLayerState PushWidgetState, UWidget_ActivatableBase* CreatedWidget)
	{
		if (PushWidgetState == EAsyncWidgetLayerState::Initialize)
		{
			UWidget_ConfirmScreen* ConfirmScreen = CastChecked<UWidget_ConfirmScreen>(CreatedWidget);
			ConfirmScreen->InitConfirmScreen(ScreenInfo, ButtonClickedCallback);
		}
	});
}

void UFrontendUISubsystem::NotifyPlayerAdded(UXYSCommonLocalPlayer* LocalPlayer)
{
	if (!LocalPlayer)
		return;

	// 确认为单人玩家游戏，不考虑其他LocalPlayer的加入
	ensureAlwaysMsgf(LocalPlayer != RootViewportLayout.LocalPlayer, TEXT("Add same LocalPlayer again!"));
	ensureAlwaysMsgf(RootViewportLayout.LocalPlayer == nullptr, TEXT("Add Another LocalPlayer!"));

	if (RootViewportLayout.LocalPlayer)
		return;
	
	LocalPlayer->OnPlayerControllerSet.AddWeakLambda(this, [this](UXYSCommonLocalPlayer* LocalPlayer, APlayerController* PlayerController)
	{
		NotifyPlayerDestroyed(LocalPlayer);

		// ensureAlwaysMsgf(LocalPlayer == RootViewportLayout.LocalPlayer, TEXT("PlayerController's LocalPlayer should same to RootViewportLayout's!"));
		if (RootViewportLayout.RootLayout)
		{
			AddLayoutToViewport(LocalPlayer, RootViewportLayout.RootLayout);
			RootViewportLayout.bAddedToViewport = true;
		}
		else
		{
			CreateLayoutWidget(LocalPlayer);
		}
	});

	if (RootViewportLayout.RootLayout)
	{
		AddLayoutToViewport(LocalPlayer, RootViewportLayout.RootLayout);
		RootViewportLayout.bAddedToViewport = true;
	}
	else
	{
		CreateLayoutWidget(LocalPlayer);
	}
}

void UFrontendUISubsystem::NotifyPlayerRemoved(UXYSCommonLocalPlayer* LocalPlayer)
{
	if (RootViewportLayout.LocalPlayer != LocalPlayer)
		return;
	
	RemoveLayoutFromViewport(LocalPlayer, RootViewportLayout.RootLayout);
	RootViewportLayout.bAddedToViewport = false;
}

UCommonActivatableWidget* UFrontendUISubsystem::PushWidgetToLayerStack(ULocalPlayer* LocalPlayer, FGameplayTag LayerName, TSubclassOf<UCommonActivatableWidget> InActivatableWidgetClass)
{
	UWidget_PrimaryLayout* PrimaryLayout = GetRootLayout(CastChecked<UXYSCommonLocalPlayer>(LocalPlayer));
	ensure(PrimaryLayout);
	return PrimaryLayout->PushWidgetToLayerStack(LayerName, InActivatableWidgetClass);
}

void UFrontendUISubsystem::PushWidgetToLayerStackAsync(ULocalPlayer* LocalPlayer, FGameplayTag LayerName,
	bool bSuspendInputUntilComplete, TSoftClassPtr<UCommonActivatableWidget> WidgetClass)
{
	UWidget_PrimaryLayout* PrimaryLayout = GetRootLayout(CastChecked<UXYSCommonLocalPlayer>(LocalPlayer));
	ensure(PrimaryLayout);
	PrimaryLayout->PushWidgetToLayerStackAsync(LayerName, bSuspendInputUntilComplete, WidgetClass);
}

void UFrontendUISubsystem::FindAndRemoveWidgetFromLayer(ULocalPlayer* LocalPlayer,
	UCommonActivatableWidget* InActivatableWidget)
{
	UWidget_PrimaryLayout* PrimaryLayout = GetRootLayout(CastChecked<UXYSCommonLocalPlayer>(LocalPlayer));
	ensure(PrimaryLayout);
	PrimaryLayout->FindAndRemoveWidgetFromLayer(InActivatableWidget);
}

UWidget_PrimaryLayout* UFrontendUISubsystem::GetRootLayout(const UXYSCommonLocalPlayer* LocalPlayer) const
{
	// const FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer);
	// return LayoutInfo ? LayoutInfo->RootLayout : nullptr;

	return RootViewportLayout.RootLayout;
}

void UFrontendUISubsystem::NotifyPlayerDestroyed(UXYSCommonLocalPlayer* LocalPlayer)
{
	NotifyPlayerRemoved(LocalPlayer);
	LocalPlayer->OnPlayerControllerSet.RemoveAll(this);
	
	if (LocalPlayer == RootViewportLayout.LocalPlayer)
	{
		UWidget_PrimaryLayout* Layout = RootViewportLayout.RootLayout;

		RemoveLayoutFromViewport(LocalPlayer, Layout);
		OnRootLayoutReleased(LocalPlayer, Layout);

		RootViewportLayout = FRootViewportLayoutInfo();
	}
}


void UFrontendUISubsystem::CreateLayoutWidget(UXYSCommonLocalPlayer* InLocalPlayer)
{
	if (APlayerController* PlayerController = InLocalPlayer->GetPlayerController(GetWorld()))
	{
		TSubclassOf<UWidget_PrimaryLayout> LayoutWidgetClass = GetLayoutWidgetClass();
		if (ensure(LayoutWidgetClass) && !LayoutWidgetClass->HasAnyClassFlags(CLASS_Abstract))
		{
			UWidget_PrimaryLayout* NewWidgetObject = CreateWidget<UWidget_PrimaryLayout>(PlayerController, LayoutWidgetClass);
			RootViewportLayout.LocalPlayer = InLocalPlayer;
			RootViewportLayout.RootLayout = NewWidgetObject;

			AddLayoutToViewport(InLocalPlayer, NewWidgetObject);
			RootViewportLayout.bAddedToViewport = true;
		}
	}
}

void UFrontendUISubsystem::AddLayoutToViewport(UXYSCommonLocalPlayer* InLocalPlayer, UWidget_PrimaryLayout* InRootLayout)
{
	UE_LOG(LogFrontend, Log, TEXT("[%s] is adding player [%s]'s root layout [%s] to the viewport"), *GetName(), *GetNameSafe(InLocalPlayer), *GetNameSafe(InRootLayout));

	InRootLayout->SetPlayerContext(FLocalPlayerContext(InLocalPlayer));
	InRootLayout->AddToPlayerScreen(1000);

	OnRootLayoutAddedToViewport(InLocalPlayer, InRootLayout);
}

void UFrontendUISubsystem::RemoveLayoutFromViewport(UXYSCommonLocalPlayer* LocalPlayer, UWidget_PrimaryLayout* Layout)
{
	TWeakPtr<SWidget> LayoutSlateWidget = Layout->GetCachedWidget();
	if (LayoutSlateWidget.IsValid())
	{
		UE_LOG(LogFrontend, Log, TEXT("[%s] is removing player [%s]'s root layout [%s] from the viewport"),
			*GetName(), *GetNameSafe(LocalPlayer), *GetNameSafe(Layout));

		Layout->RemoveFromParent();
		if (LayoutSlateWidget.IsValid())
		{
			UE_LOG(LogFrontend, Log,
				TEXT("Player [%s]'s root layout [%s] has been removed from the viewport, but other references to its underlying Slate widget still exist. Noting in case we leak it."), *GetNameSafe(LocalPlayer), *GetNameSafe(Layout));
		}

		OnRootLayoutRemovedFromViewport(LocalPlayer, Layout);
	}
}


TSubclassOf<UWidget_PrimaryLayout> UFrontendUISubsystem::GetLayoutWidgetClass() const
{
	TSoftClassPtr<UWidget_PrimaryLayout> WidgetClass = UFrontendBlueprintFunctionLibrary::GetPrimaryLayoutWidgetClass();
	ensureAlwaysMsgf(!WidgetClass.IsNull(), TEXT("Make sure PrimaryLayout widget class is not null."));

	return WidgetClass.LoadSynchronous();
}

void UFrontendUISubsystem::OnRootLayoutAddedToViewport(UXYSCommonLocalPlayer* LocalPlayer, UWidget_PrimaryLayout* Layout)
{
#if WITH_EDITOR
	if (GIsEditor && LocalPlayer->IsPrimaryPlayer())
	{
		// So our controller will work in PIE without needing to click in the viewport
		FSlateApplication::Get().SetUserFocusToGameViewport(0);
	}
#endif
	OnPrimaryLayoutAdded.Broadcast();
}

void UFrontendUISubsystem::OnRootLayoutRemovedFromViewport(UXYSCommonLocalPlayer* LocalPlayer, UWidget_PrimaryLayout* Layout)
{
}

void UFrontendUISubsystem::OnRootLayoutReleased(UXYSCommonLocalPlayer* LocalPlayer, UWidget_PrimaryLayout* Layout)
{
}
