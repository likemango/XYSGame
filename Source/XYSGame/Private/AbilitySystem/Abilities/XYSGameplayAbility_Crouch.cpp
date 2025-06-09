// XiaoYao copyright.


#include "AbilitySystem/Abilities/XYSGameplayAbility_Crouch.h"

#include "Character/XYSCharacter.h"
#include "Character/XYSCharacterMovementComponent.h"

UXYSGameplayAbility_Crouch::UXYSGameplayAbility_Crouch(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	
}

bool UXYSGameplayAbility_Crouch::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
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
	if (XYSCharacter->bIsCrouched || XYSMovementComponent->bWantsToCrouch)
	{
		return false;
	}

	if (XYSMovementComponent->IsMovingOnGround())
	{
		return true;
	}
	
	return false;
}

void UXYSGameplayAbility_Crouch::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	// Stop crouching in case the ability blueprint doesn't call it.
	CharacterCrouchEnd();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UXYSGameplayAbility_Crouch::CharacterCrouchStart()
{
	if (AXYSCharacter* XYSCharacter = GetXYSCharacterFromActorInfo())
	{
		if (XYSCharacter->IsLocallyControlled())
		{
			XYSCharacter->Crouch();
		}
	}
}

void UXYSGameplayAbility_Crouch::CharacterCrouchEnd()
{
	if (AXYSCharacter* XYSCharacter = GetXYSCharacterFromActorInfo())
	{
		if (XYSCharacter->IsLocallyControlled())
		{
			XYSCharacter->UnCrouch();
		}
	}
}
