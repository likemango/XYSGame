// Copyright 2021 - 2024, Kia Armani. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameplayAbilitySpec.h"
#include "MotionInputConfig.h"
#include "Abilities/GameplayAbility.h"
#include "MotionCore/AbilitySystem/MotionAbilitySystemComponent.h"
#include "MotionCore/Character/MotionCharacter.h"
#include "MotionInputComponent.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UInputAction;
class UObject;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInputActionDelegate);

/**
 * 
 */
UCLASS()
class MOTIONCORE_API UMotionInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	explicit UMotionInputComponent(const FObjectInitializer& ObjectInitializer);

	template<class UserClass, typename FuncType>
	void BindActionByTag(const UMotionInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func);

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UMotionInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);
};

template<class UserClass, typename FuncType>
	void UMotionInputComponent::BindActionByTag(const UMotionInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
{
	check(InputConfig);
	if (const UInputAction* Ia = InputConfig->FindInputActionForTag(InputTag))
	{
		BindAction(Ia, TriggerEvent, Object, Func);
	}
}

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UMotionInputComponent::BindAbilityActions(const UMotionInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	check(InputConfig);

	for (const FMotionGameplayAbility& Ability : InputConfig->TaggedAbilityActions)
	{
		if (Ability.InputAction && Ability.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindHandles.Add(BindAction(Ability.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Ability.InputTag).GetHandle());
			}

			if (ReleasedFunc)
			{
				BindHandles.Add(BindAction(Ability.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Ability.InputTag).GetHandle());
			}
		}
		
		UGameplayAbility* AbilityCDO = Ability.Ability->GetDefaultObject<UGameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, 1);
		AbilitySpec.SourceObject = this;
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(Ability.InputTag);

		UMotionAbilitySystemComponent* MotionAbilitySystemComponent = Cast<UMotionAbilitySystemComponent>(Cast<AMotionCharacter>(GetOwner())->GetAbilitySystemComponent());
		MotionAbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}
