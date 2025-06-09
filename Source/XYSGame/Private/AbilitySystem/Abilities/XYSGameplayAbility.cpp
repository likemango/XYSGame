// XiaoYao copyright.


#include "AbilitySystem/Abilities/XYSGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "Character/XYSCharacter.h"
#include "Character/XYSCharacterMovementComponent.h"

UXYSGameplayAbility::UXYSGameplayAbility(const FObjectInitializer& ObjectInitializer)
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
	
	ActivationPolicy = EXYSAbilityActivationPolicy::OnInputTriggered;
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

