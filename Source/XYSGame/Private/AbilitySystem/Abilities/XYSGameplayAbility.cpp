// XiaoYao copyright.


#include "AbilitySystem/Abilities/XYSGameplayAbility.h"

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

AXYSCharacter* UXYSGameplayAbility::GetXYSCharacterFromActorInfo() const
{
	return CurrentActorInfo ? Cast<AXYSCharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr;
}

UXYSCharacterMovementComponent* UXYSGameplayAbility::GetXYSCharacterMovementFromActorInfo() const
{
	return CurrentActorInfo ? Cast<UXYSCharacterMovementComponent>(CurrentActorInfo->MovementComponent.Get()) : nullptr;
}

