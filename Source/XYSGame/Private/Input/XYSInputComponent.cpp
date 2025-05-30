#include "Input/XYSInputComponent.h"

UXYSInputComponent::UXYSInputComponent(const FObjectInitializer& ObjectInitializer)
{
}

void UXYSInputComponent::AddInputMappings(const UXYSInputConfig* InputConfig,
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	// Here you can handle any custom logic to add something from your input config if required
	// 可以添加其他自定义在InputConfig里的输入配置
}

void UXYSInputComponent::RemoveInputMappings(const UXYSInputConfig* InputConfig,
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);
	
	// Here you can handle any custom logic to remove input mappings that you may have added above
}

void UXYSInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Hanlde : BindHandles)
	{
		RemoveBindingByHandle(Hanlde);
	}
	BindHandles.Reset();
}
