// XiaoYao copyright.


#include "AbilitySystem/Attributes/XYSCombatSet.h"

#include "Net/UnrealNetwork.h"

class FLifetimeProperty;


UXYSCombatSet::UXYSCombatSet()
	: BaseDamage(0.0f)
	, BaseHeal(0.0f)
{
}

void UXYSCombatSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UXYSCombatSet, BaseDamage, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UXYSCombatSet, BaseHeal, COND_OwnerOnly, REPNOTIFY_Always);
}

void UXYSCombatSet::OnRep_BaseDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UXYSCombatSet, BaseDamage, OldValue);
}

void UXYSCombatSet::OnRep_BaseHeal(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UXYSCombatSet, BaseHeal, OldValue);
}

