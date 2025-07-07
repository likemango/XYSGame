// XiaoYao copyright.


#include "Subsystem/FrontendUISubsystem.h"

#include "FrontendGameplayTags.h"
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
	TFunction<void(EAsyncPushWidgetState, UWidget_ActivatableBase*)> AsyncPushWidgetCallback) 
{
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(InSoftWidgetClass.ToSoftObjectPath(), FStreamableDelegate::CreateLambda(
		[InSoftWidgetClass, this, InTag, AsyncPushWidgetCallback]()
		{
			UClass* LoadedClass = InSoftWidgetClass.Get();
			check(CreatedPrimaryLayout && LoadedClass);
			UCommonActivatableWidgetContainerBase* ContainBase = CreatedPrimaryLayout->FindWidgetStackByTag(InTag);
			check(ContainBase);
			UWidget_ActivatableBase* CreatedPushedWidget = ContainBase->AddWidget<UWidget_ActivatableBase>(LoadedClass, [AsyncPushWidgetCallback](UWidget_ActivatableBase& CreatedWidget)
			{
				AsyncPushWidgetCallback(EAsyncPushWidgetState::OnCreatedBeforePush, &CreatedWidget);
			});
			AsyncPushWidgetCallback(EAsyncPushWidgetState::AfterPush, CreatedPushedWidget);
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
	TSoftClassPtr<UWidget_ActivatableBase> ConfirmScreenWidgetClass = UFrontendBlueprintFunctionLibrary::GetFrontendSoftWidgetClassFromDevelopSettingsByTag(FrontendGameplayTags::Frontend_Widget_ConfirmScreen);
	PushSoftWidgetClassToStackAsync(FrontendGameplayTags::Frontend_WidgetStack_Modal, ConfirmScreenWidgetClass,
		[ScreenInfo, ButtonClickedCallback](EAsyncPushWidgetState PushWidgetState, UWidget_ActivatableBase* CreatedWidget)
	{
		if (PushWidgetState == EAsyncPushWidgetState::OnCreatedBeforePush)
		{
			UWidget_ConfirmScreen* ConfirmScreen = CastChecked<UWidget_ConfirmScreen>(CreatedWidget);
			ConfirmScreen->InitConfirmScreen(ScreenInfo, ButtonClickedCallback);
		}
	});
}


void UFrontendUISubsystem::RegisterCreatedPrimaryLayout(UWidget_PrimaryLayout* InLayout)
{
	check(InLayout);

	CreatedPrimaryLayout = InLayout;
}
