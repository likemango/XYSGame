// XiaoYao copyright.


#include "Player/XYSPlayerState.h"

#include "AbilitySystem/XYSAbilitySystemComponent.h"

AXYSPlayerState::AXYSPlayerState(const FObjectInitializer& ObjectInitializer)
{

	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UXYSAbilitySystemComponent>(this, "AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAttributeSet>("AttributeSet");

	SetNetUpdateFrequency(100);
}

UAbilitySystemComponent* AXYSPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AXYSPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}
