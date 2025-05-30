// XiaoYao copyright.


#include "AbilitySystem/Abilities/XYSGameplayAbility.h"

UXYSGameplayAbility::UXYSGameplayAbility(const FObjectInitializer& ObjectInitializer)
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
	
	ActivationPolicy = EXYSAbilityActivationPolicy::OnInputTriggered;
}
