// XiaoYao copyright.


#include "AbilitySystem/Abilities/XYSGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemLog.h"
#include "AbilitySystem/XYSAbilitySystemComponent.h"
#include "Character/XYSCharacter.h"
#include "Character/XYSCharacterMovementComponent.h"
#include "Character/XYSHeroComponent.h"

#define ENSURE_ABILITY_IS_INSTANTIATED_OR_RETURN(FunctionName, ReturnValue)																				\
{																																						\
if (!ensure(IsInstantiated()))																														\
{																																					\
ABILITY_LOG(Error, TEXT("%s: " #FunctionName " cannot be called on a non-instanced ability. Check the instancing policy."), *GetPathName());	\
return ReturnValue;																																\
}																																					\
}

UXYSGameplayAbility::UXYSGameplayAbility(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;

	ActivationPolicy = EXYSAbilityActivationPolicy::OnInputTriggered;
	ActivationGroup = EXYSAbilityActivationGroup::Independent;
}

void UXYSGameplayAbility::OnPawnAvatarSet()
{
	K2_OnPawnAvatarSet();
}

void UXYSGameplayAbility::TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec) const
{
	// Try to activate if activation policy is on spawn.
	if (ActorInfo && !Spec.IsActive() && (ActivationPolicy == EXYSAbilityActivationPolicy::OnSpawn))
	{
		UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
		const AActor* AvatarActor = ActorInfo->AvatarActor.Get();

		// If avatar actor is torn off or about to die, don't try to activate until we get the new one.
		if (ASC && AvatarActor && !AvatarActor->GetTearOff() && (AvatarActor->GetLifeSpan() <= 0.0f))
		{
			const bool bIsLocalExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalPredicted) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalOnly);
			const bool bIsServerExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerOnly) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerInitiated);

			const bool bClientShouldActivate = ActorInfo->IsLocallyControlled() && bIsLocalExecution;
			const bool bServerShouldActivate = ActorInfo->IsNetAuthority() && bIsServerExecution;

			if (bClientShouldActivate || bServerShouldActivate)
			{
				ASC->TryActivateAbility(Spec.Handle);
			}
		}
	}
}

AXYSCharacter* UXYSGameplayAbility::GetXYSCharacterFromActorInfo() const
{
	return CurrentActorInfo ? Cast<AXYSCharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr;
}

UXYSCharacterMovementComponent* UXYSGameplayAbility::GetXYSCharacterMovementFromActorInfo() const
{
	return CurrentActorInfo ? Cast<UXYSCharacterMovementComponent>(CurrentActorInfo->MovementComponent.Get()) : nullptr;
}

UXYSAbilitySystemComponent* UXYSGameplayAbility::GetXYSAbilitySystemComponentFromActorInfo() const
{
	return CurrentActorInfo ? Cast<UXYSAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get()) : nullptr;
}

AController* UXYSGameplayAbility::GetControllerFromActorInfo() const
{
	if (CurrentActorInfo)
	{
		if (AController* PC = CurrentActorInfo->PlayerController.Get())
		{
			return PC;
		}

		// Look for a player controller or pawn in the owner chain.
		AActor* TestActor = CurrentActorInfo->OwnerActor.Get();
		while (TestActor)
		{
			if (AController* C = Cast<AController>(TestActor))
			{
				return C;
			}

			if (APawn* Pawn = Cast<APawn>(TestActor))
			{
				return Pawn->GetController();
			}

			TestActor = TestActor->GetOwner();
		}
	}

	return nullptr;
}

UXYSHeroComponent* UXYSGameplayAbility::GetHeroComponentFromActorInfo() const
{
	return (CurrentActorInfo ? UXYSHeroComponent::FindHeroComponent(CurrentActorInfo->AvatarActor.Get()) : nullptr);
	
}

bool UXYSGameplayAbility::CanChangeActivationGroup(EXYSAbilityActivationGroup NewGroup) const
{
	if (!IsInstantiated() || !IsActive())
	{
		return false;
	}

	if (ActivationGroup == NewGroup)
	{
		return true;
	}

	UXYSAbilitySystemComponent* XYSASC = GetXYSAbilitySystemComponentFromActorInfo();
	check(XYSASC);

	if ((ActivationGroup != EXYSAbilityActivationGroup::Exclusive_Blocking) && XYSASC->IsActivationGroupBlocked(NewGroup))
	{
		// This ability can't change groups if it's blocked (unless it is the one doing the blocking).
		return false;
	}

	if ((NewGroup == EXYSAbilityActivationGroup::Exclusive_Replaceable) && !CanBeCanceled())
	{
		// This ability can't become replaceable if it can't be canceled.
		return false;
	}

	return true;
}

bool UXYSGameplayAbility::ChangeActivationGroup(EXYSAbilityActivationGroup NewGroup)
{
	ENSURE_ABILITY_IS_INSTANTIATED_OR_RETURN(ChangeActivationGroup, false);

	if (!CanChangeActivationGroup(NewGroup))
	{
		return false;
	}

	if (ActivationGroup != NewGroup)
	{
		UXYSAbilitySystemComponent* XYSASC = GetXYSAbilitySystemComponentFromActorInfo();
		check(XYSASC);

		XYSASC->RemoveAbilityFromActivationGroup(ActivationGroup, this);
		XYSASC->AddAbilityToActivationGroup(NewGroup, this);

		ActivationGroup = NewGroup;
	}

	return true;
}

void UXYSGameplayAbility::NativeOnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const
{
	// todo
}

