// Copyright 2021 - 2024, Kia Armani. All Rights Reserved.

#include "MotionInputConfig.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MotionInputConfig)


UMotionInputConfig::UMotionInputConfig(const FObjectInitializer& ObjectInitializer)
{
}

const UInputAction* UMotionInputConfig::FindInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FMotionInputAction& TaggedInputAction : TaggedNativeActions)
	{
		if (TaggedInputAction.InputAction && TaggedInputAction.InputTag == InputTag)
		{
			return TaggedInputAction.InputAction;
		}
	}
	return nullptr;
}

const UInputAction* UMotionInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
                                                                     const bool bLogNotFound) const
{
	for (const FMotionGameplayAbility& Action : TaggedAbilityActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find TaggedAbilityAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}
