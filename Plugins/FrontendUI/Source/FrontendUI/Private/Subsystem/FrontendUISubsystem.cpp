// XiaoYao copyright.


#include "Subsystem/FrontendUISubsystem.h"

#include "DebugHelper.h"
#include "Engine/AssetManager.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "Widgets/Widget_ActivatableBase.h"
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
	TSoftClassPtr<UWidget_ActivatableBase>& InSoftWidgetClass,
	TFunction<void(EAsyncPushWidgetState, UWidget_ActivatableBase*)> AsyncPushWidgetCallback) const
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



void UFrontendUISubsystem::RegisterCreatedPrimaryLayout(UWidget_PrimaryLayout* InLayout)
{
	check(InLayout);

	CreatedPrimaryLayout = InLayout;

	DebugHelper::Print(TEXT("PrimaryLayout widget stored by FrontendUISubsystem."));
}
