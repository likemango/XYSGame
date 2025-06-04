// XiaoYao copyright.


#include "AbilitySystem/Abilities/XYSGameplayAbility_Jump.h"

#include "Character/XYSCharacter.h"
#include "Character/XYSCharacterMovementComponent.h"

UXYSGameplayAbility_Jump::UXYSGameplayAbility_Jump(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool UXYSGameplayAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                  const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                  const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		return false;
	}
	
	AXYSCharacter* XYSCharacter = GetXYSCharacterFromActorInfo();
	if (!XYSCharacter)
	{
		return false;
	}

	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	UXYSCharacterMovementComponent* XYSMovementComponent = GetXYSCharacterMovementFromActorInfo();
	// crouch时允许通过jump切换回站立
	if (XYSCharacter->bIsCrouched || XYSMovementComponent->bWantsToCrouch)
	{
		return true;
	}

	// crouch时判定为不能够jump，因此crouch状态提前判断
	if (!XYSCharacter->CanJump())
	{
		return false;
	}
	
	return true;
}

void UXYSGameplayAbility_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	// Stop jumping in case the ability blueprint doesn't call it. 
	CharacterJumpEnd();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UXYSGameplayAbility_Jump::CharacterJumpStart()
{
	if (AXYSCharacter* XYSCharacter = GetXYSCharacterFromActorInfo())
	{
		if (XYSCharacter->IsLocallyControlled() && !XYSCharacter->bPressedJump)
		{
			if (UXYSCharacterMovementComponent* XYSMovementComponent = GetXYSCharacterMovementFromActorInfo())
			{
				if (XYSCharacter->bIsCrouched || XYSMovementComponent->bWantsToCrouch)
				{
					XYSCharacter->UnCrouch();
				}
				else
				{
					XYSCharacter->Jump();
				}
			}
		}
	}
}

void UXYSGameplayAbility_Jump::CharacterJumpEnd()
{
	if (AXYSCharacter* XYSCharacter = GetXYSCharacterFromActorInfo())
	{
		if (XYSCharacter->IsLocallyControlled() && XYSCharacter->bPressedJump)
		{
			XYSCharacter->StopJumping();
		}
	}
}
