// XiaoYao copyright.


#include "Widgets/Widget_PrimaryLayout.h"

#include "DebugHelper.h"
#include "GameplayTagContainer.h"

UCommonActivatableWidgetContainerBase* UWidget_PrimaryLayout::FindWidgetStackByTag(const FGameplayTag& InTag) const
{
	ensureAlwaysMsgf(RegisteredWidgetStackMap.Contains(InTag), TEXT("Can not find the widget stack by the tag %s"), *InTag.ToString());
	if (InTag.IsValid())
	{
		return RegisteredWidgetStackMap.FindRef(InTag);
	}
	return nullptr;
}

void UWidget_PrimaryLayout::RegisterWidgetStack(FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* InStack)
{
	check(InStack);

	if (!IsDesignTime())
	{
		if (!RegisteredWidgetStackMap.Contains(InStackTag))
		{
			RegisteredWidgetStackMap.Add(InStackTag, InStack);

			DebugHelper::Print(TEXT("Registered widget stack with tag ") + InStackTag.ToString());
		}
	}
}
