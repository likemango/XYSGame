// XiaoYao copyright.


#include "System/XYSGameInstance.h"

#include "XYSGameplayTags.h"
#include "Components/GameFrameworkComponentManager.h"

UXYSGameInstance::UXYSGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UXYSGameInstance::Init()
{
	Super::Init();

	// Register our custom init states
	UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);

	if (ensure(ComponentManager))
	{
		ComponentManager->RegisterInitState(XYSGameplayTags::InitState_Spawned, false, FGameplayTag());
		ComponentManager->RegisterInitState(XYSGameplayTags::InitState_DataAvailable, false, XYSGameplayTags::InitState_Spawned);
		ComponentManager->RegisterInitState(XYSGameplayTags::InitState_DataInitialized, false, XYSGameplayTags::InitState_DataAvailable);
		ComponentManager->RegisterInitState(XYSGameplayTags::InitState_GameplayReady, false, XYSGameplayTags::InitState_DataInitialized);
	}
}
